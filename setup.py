import os
import re
import subprocess
import sys

from skbuild import setup

from pathlib import Path
this_directory = Path(__file__).parent
long_description = (this_directory / "README.md").read_text()

about = {}
with open("src/vivid3d/_details.py") as fp:
    exec(fp.read(), about)

setup(
    name="vivid3d",
    version=about['__version__'],
    packages=['vivid3d', 'vivid3d.viewer'],
    package_dir={'': 'src'},
    package_data={'vivid3d.viewer': ["template.html"]},
    #include_package_data=True,
    cmake_install_dir='src/vivid3d',
    author='GalaxyHuntersIL',
    author_email='galaxyhuntersil@gmail.com',
    url='https://galaxyhuntersil.wixsite.com/website/vivid',
    project_urls={
        "Documentation": "https://vivid.readthedocs.io/en/latest/",
        "Bug Tracker": "https://github.com/GalaxyHunters/Vivid/issues",
        "Source Code": "https://github.com/GalaxyHunters/Vivid",
    },
    description='A 3D visualisation tool for simulations',
    long_description=long_description,
    long_description_content_type='text/markdown',
    zip_safe=False,
    # Needed at import time: the compiled module imports numpy, and the viewers import matplotlib
    # (also used for colormaps; matplotlib.colormaps needs >= 3.5). Without these `import vivid3d`
    # fails with "ImportError: initialization failed".
    install_requires=["numpy>=1.21.3", "matplotlib>=3.5"],
    extras_require={
        # only needed for the in-notebook viewers (imported lazily)
        "notebook": ["ipywidgets", "ipython"],
        "test": ["pytest>=6.0"],
    },
    # Tested with tools/golden on CPython 3.10 - 3.14 (3.10 also with the minimum numpy/matplotlib).
    # Older versions are end-of-life and untested; 3.10 needed a fix (CPython 3.10's headers define a
    # global `CFrame` that clashed with vivid::CFrame in the wrapper).
    python_requires=">=3.10",
    keywords=["3d", "mesh", "visualization", "simulation", "particles"],
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Science/Research",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: BSD License",
        "Programming Language :: C++",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
        "Programming Language :: Python :: 3.12",
        "Programming Language :: Python :: 3.13",
        "Programming Language :: Python :: 3.14",
        "Programming Language :: Python :: Implementation :: CPython",
        "Topic :: Software Development",
        "Topic :: Scientific/Engineering",
        "Topic :: Multimedia :: Graphics :: 3D Modeling",
        "Typing :: Typed",
        "Operating System :: Microsoft :: Windows",
        "Operating System :: POSIX",
        "Operating System :: Unix",
        "Operating System :: MacOS",
    ]
)
