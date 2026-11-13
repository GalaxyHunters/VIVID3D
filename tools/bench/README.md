# tools/bench - runtime and memory benchmark of the real VIVID flow

Measures `points -> VoronoiVolume -> iso_surface (3 density masks) -> smooth -> reduce (decimate) -> export` on a
seeded synthetic spiral galaxy (dense disk with arms, bulge, sparse halo, a density field spanning ~6 orders of
magnitude and a colour metric), at any size. Only numpy and the vivid3d under test are needed.

```
python bench.py run   --python <py with vivid3d> --n 1e6 --label mono-1e6
python bench.py scale --python <py> --sizes 1e4,1e5,3e5,1e6,3e6 --predict 1e7,2e7 --mem-cap-gb 30
python refdump.py dump --python <old py> --out ref_old     # exact-output check, see below
python refdump.py dump --python <new py> --out ref_new
python refdump.py compare ref_old ref_new
```

* Every size runs in a fresh child process. The parent samples the child's resident memory every 0.1 s (the wrapper
  holds the GIL in long C++ calls, so a sampler inside the child would starve), enforces `--mem-cap-gb` and
  `--timeout`, and attributes the peak to the stage that was running. `scale` fits a power law per stage and
  predicts bigger sizes.
* `refdump.py` is the safety net for refactors: it exports every mask's mesh (raw and after smooth + reduce) to OBJ
  for seeded scenes and requires the new build's files to be **byte identical** to the old build's. Compare builds of
  the same platform. Optimisations 1 and 2 below passed it on Windows (MSVC) and Linux (gcc).
* `--chunks K` splits one model into K spatial slabs (each with its own Voronoi). It is a diagnostic only: it creates
  seams, so it is not a way to get the same model.

## Results (Windows, 20 threads, synthetic galaxy, 3 density masks; `stable-2026`-era code -> 1.1.0 optimised)

| points | time before | time after | peak memory before | peak memory after |
|---|---|---|---|---|
| 1e5 | 23.7 s | 8.8 s (2.7x) | 830 MB | 504 MB (-39%) |
| 3e5 | 75 s | 28 s (2.7x) | 2.2 GB | 1.3 GB (-40%) |
| 1e6 | 286 s | 80 s (3.6x) | 7.2 GB | 4.2 GB (-42%) |
| 3e6 | 1009 s | 280 s (3.6x) | 21.8 GB | 12.3 GB (-44%) |

Wall times on a desktop vary by up to ~30% between runs (the same 1e6 run took 80 s and 105 s on two occasions), so
read the speed-up as 2.7x - 3.6x; the memory figures are stable. Fitted growth (time ~ n^1.03, peak memory ~ n^0.81)
predicts about 16 min and 26 GB for 1e7 points and 33 min and 45 GB for 2e7 (before: 63 min / 53 GB and 136 min /
96 GB, which did not fit a 64 GB machine). Real data differs from the synthetic galaxy; treat the numbers as a guide.

## What was changed (all output-identical)

1. **iso_surface** (`src/ModelBuilder/VoronoiVolume.*`): it deep-copied the whole Voronoi volume (a heap object per
   vertex, atomic reference counts, pointer-keyed `std::map`s) for every mask and only then discarded >99% of the
   faces. The volume now stores flat index arrays; `MaskMesh` is `const`, scans the faces once and touches only the
   faces on the surface. iso_surface 477 s -> 43 s at 3e6 points; the Voronoi stage also became 2.4x faster.
2. **Decimation** (`lib/Decimate.*`, `lib/MeshDecimation`): a `std::set` was allocated on every edge-cost evaluation
   (~46 per collapse), and the glue copied the mesh several times. Now no allocation in the hot path, the mesh is
   passed by reference and replaced in place, and the decimator's memory is freed before the output is built.

## Possible future upgrades (measured candidates, none done yet)

Where the time goes now (3e6 points): decimation ~58%, Voronoi ~18%, export ~12%, iso_surface ~9%, smooth ~3%.
Inside decimation: `ComputeEdgeCost` 54% (15M calls for 330k collapses), stale priority-queue pops 25%, initial queue
fill 10%, the collapses themselves 6%.

| idea | expected gain | risk / cost |
|---|---|---|
| Release the GIL in `iso_surface` / `smooth` / `reduce` / Voronoi, so independent masks run on separate threads | ~2.5-3x on the per-mask stages for a 3-mask flow, identical output | the console log sink calls Python and must re-acquire the GIL; check the logger's thread safety |
| Skip priority-queue pushes whose edge cost did not change | maybe ~2x on decimation, a much smaller queue (it ends at millions of entries) | tie-breaking between equal-cost edges changes: equivalent but not byte-identical output; needs a quality comparison first |
| Voronoi stage memory (it holds ~10 of the 13 GB peak at 3e6): `CVoronoiVolume` keeps the whole tessellation (`mVoronoi.mData`) alive, `GetFaces()` builds a vector of vectors, numpy -> `vector<CPoint>` conversion | large memory cut | the Voronoi library is research code nobody on the team knows: freeze its output with characterisation tests first |
| Cheaper smoothing (`LaplacianSmooth` / `CalculatePointsNeighbours` use `std::map`/`std::set`) and a faster exporter (export grows ~n^1.25) | a few % each | low |
| Cache locality of the decimator (renumber vertices/edges, smaller vertex records; ~350 B per vertex today) | decimation is memory bound at 1e6 | moderate |
| `EdgeCollapse` boundary update reads `idEdge` before it is assigned (first iteration) | correctness, not speed | changes the output; fix deliberately |
| `CMesh::Reduce` does not refresh the neighbour table used by `smooth` | correctness if `smooth` runs after `reduce` | verify first |
| Validate with a real AMR galaxy (the public yt sample `IsolatedGalaxy`) and the 10-20M target | confidence | needs `yt` in a throwaway venv |

Profiling notes: valgrind/callgrind works on the Linux VM, but instruction counts mislead for the memory-bound
decimator (restructuring `ManifoldConstraint` cut instructions without changing wall time and was dropped);
`perf` needs `kernel.perf_event_paranoid` lowered. Measure wall time at 1e5-1e6 before and after every change.
