import os
import re
import subprocess
import sys

from skbuild import setup

from pathlib import Path
this_directory = Path(__file__).parent
long_description = (this_directory / "README.md").read_text()

setup(
    name="vivid3d",
    version="0.1.4",
    packages=['vivid3d'],
    package_dir={'': 'src'},
    cmake_install_dir='src/vivid3d',
    author='GalaxyHuntersIL',
    author_email='galaxyhuntersil@gmail.com',
    url='https://galaxyhuntersil.wixsite.com/labs/vivid',
    description='A 3D visualisation tool for simulations',
    long_description=long_description,
    long_description_content_type='text/markdown',
    zip_safe=False,
    extras_require={"test": ["pytest>=6.0"]},
    python_requires=">=3.6"
)
