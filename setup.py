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
    include_package_data=True,
    cmake_install_dir='src/vivid3d',
    author='GalaxyHuntersIL',
    author_email='galaxyhuntersil@gmail.com',
    url='https://galaxyhuntersil.wixsite.com/labs/vivid',
    project_urls={
        "Documentation": "https://vivid.readthedocs.io/en/latest/",
        "Bug Tracker": "https://github.com/GalaxyHunters/Vivid/issues",
        "Source Code": "https://github.com/GalaxyHunters/Vivid",
    },
    description='A 3D visualisation tool for simulations',
    long_description=long_description,
    long_description_content_type='text/markdown',
    zip_safe=False,
    extras_require={"test": ["pytest>=6.0"]},
    python_requires=">=3.7",
    keywords=["3d", "mesh", "visualization", "simulation", "particles"],
    classifiers=[
        "Development Status :: 4 - Beta",
        "Intended Audience :: Science/Research",
        "Intended Audience :: Developers",
        "License :: OSI Approved :: BSD License",
        "Programming Language :: C++",
        "Programming Language :: Python :: 3.7",
        "Programming Language :: Python :: 3.8",
        "Programming Language :: Python :: 3.9",
        "Programming Language :: Python :: 3.10",
        "Programming Language :: Python :: 3.11",
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
