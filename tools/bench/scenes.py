"""Synthetic galaxy scenes for benchmarking the VIVID pipeline at any size.

The point distribution, density field and colour metric imitate a gas simulation of a spiral galaxy
(dense thin disk with spiral arms, bulge, sparse extended halo): most points sit where the matter is,
exactly like the cells of an AMR simulation, and the density spans about six orders of magnitude.
Everything is seeded, so a scene of a given (n, seed) is identical on every machine.

galaxy(n, seed) -> dict(points (n,3) float64 kpc, density (n,) g/cm^3-like, metric (n,) in [0,1])
masks(scene, fractions) -> list of boolean masks, each keeping roughly that fraction of the points,
                           densest first (the "density impacts which parts of the galaxy are modelled" cuts)
"""
import numpy as np


def galaxy(n, seed=0):
    n = int(n)
    rng = np.random.default_rng(seed)
    n_disk, n_bulge = int(0.55 * n), int(0.10 * n)
    n_halo = n - n_disk - n_bulge

    # disk: exponential-ish radius, two logarithmic spiral arms holding 70% of the stars/gas, thin sech-like height
    R = rng.gamma(2.0, 1.8, n_disk)
    R = np.where(R > 20.0, R % 20.0 + 0.05, R) + 0.05
    arm = rng.integers(0, 2, n_disk) * np.pi
    theta = arm + 2.2 * np.log(R + 1.0) + rng.normal(0.0, 1.0, n_disk) * (0.35 + 0.03 * R)
    uniform = rng.random(n_disk) < 0.30
    theta[uniform] = rng.uniform(0.0, 2 * np.pi, uniform.sum())
    zd = rng.laplace(0.0, 1.0, n_disk) * 0.25 * (1.0 + 0.05 * R)
    disk = np.column_stack([R * np.cos(theta), R * np.sin(theta), zd])

    bulge = rng.normal(0.0, 0.9, (n_bulge, 3)) * np.array([1.0, 1.0, 0.7])

    # halo: number density ~ r^-2 between 1 and 40 kpc, isotropic
    r = rng.uniform(1.0, 40.0, n_halo)
    d = rng.normal(0.0, 1.0, (n_halo, 3))
    d /= np.linalg.norm(d, axis=1)[:, None]
    halo = d * r[:, None]

    pts = np.vstack([disk, bulge, halo])
    del disk, bulge, halo, d, r
    pts = pts[rng.permutation(n)]            # simulation output is not sorted by component

    x, y, z = pts[:, 0], pts[:, 1], pts[:, 2]
    Rc = np.hypot(x, y)
    rr = np.sqrt(Rc * Rc + z * z)
    ang = np.arctan2(y, x)
    arms = 1.0 + 0.8 * np.cos(2.0 * (ang - 2.2 * np.log(Rc + 1.0)))
    rho = (np.exp(-Rc / 3.0) / np.cosh(z / 0.3) ** 2 * arms          # disk
           + 2.0 * np.exp(-(rr / 1.0) ** 2)                          # bulge
           + 0.02 / (1.0 + (rr / 2.0) ** 2))                         # halo
    rho = 1e-22 * rho * rng.lognormal(0.0, 0.4, n)                   # turbulence-like scatter
    rho = np.maximum(rho, 1e-28)

    # colour metric (temperature / metallicity like): cold where dense, a radial gradient and a little noise
    metric = 1.0 / (1.0 + np.exp(0.8 * (np.log10(rho) + 24.0)))
    metric = np.clip(0.75 * metric + 0.25 * np.exp(-rr / 10.0) + rng.normal(0.0, 0.03, n), 0.0, 1.0)
    return {"points": pts, "density": rho, "metric": metric}


def masks(scene, fractions=(0.05, 0.20, 0.50), sample=1_000_000, seed=1):
    """One boolean mask per fraction: the `fraction` densest points (threshold from a sample, so it is cheap for huge n)."""
    rho = scene["density"]
    rng = np.random.default_rng(seed)
    s = rho if rho.size <= sample else rho[rng.integers(0, rho.size, sample)]
    return [rho > np.quantile(s, 1.0 - f) for f in fractions]
