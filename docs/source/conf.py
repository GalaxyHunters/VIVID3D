# Configuration file for the Sphinx documentation builder.
#
# This file only contains a selection of the most common options. For a full
# list see the documentation:
# https://www.sphinx-doc.org/en/master/usage/configuration.html

# -- Path setup --------------------------------------------------------------

# If extensions (or modules to document with autodoc) are in another directory,
# add these directories to sys.path here. If the directory is relative to the
# documentation root, use os.path.abspath to make it absolute, like shown here.
#
import os
import sys
sys.path.insert(0, os.path.abspath('.'))

import vivid3d
print(dir(vivid3d.viewer))
print(dir(vivid3d.viewer.model_viewer))
print(dir(vivid3d.viewer.volumetric_viewer))

# -- Project information -----------------------------------------------------

project = 'VIVID3D'
copyright = '2022, GalaxyHuntersIL'
author = 'GalaxyHuntersIL'

# The full version, including alpha/beta/rc tags
release = vivid3d.__version__

#package_path = os.path.abspath('../..')
#os.environ['PYTHONPATH'] = ':'.join((package_path, os.environ.get('PYTHONPATH', '')))

# -- General configuration ---------------------------------------------------

# Add any Sphinx extension module names here, as strings. They can be
# extensions coming with Sphinx (named 'sphinx.ext.*') or your custom
# ones.
extensions = [
    "sphinx.ext.autodoc",
    "sphinx.ext.intersphinx",
    "sphinx.ext.autosectionlabel",
    "sphinx.ext.napoleon",
    "sphinx_mdinclude",
    'nbsphinx',
    "jupyter_sphinx",
]
autodoc_typehints_format = 'fully-qualified'
autodoc_member_order = 'bysource'
autosectionlabel_prefix_document = True
autosectionlabel_maxdepth = 3
napoleon_numpy_docstring = True
napoleon_use_rtype = False
jupyter_execute_kwargs = dict(timeout=-1, allow_errors=True)

import re

def autodoc_skip_properties(app, what, name, obj, skip, options):
    if isinstance(obj, property):
        return True
    return None

def autodoc_remove_init_methods(app, what, name, obj, options, lines):
    if what == "class":
        remove = len(lines)
        for i, line in enumerate(lines):
            if line.count(".. method::"):
                remove = i
                break

        del lines[remove:]
    return None

def setup(app):
    app.connect('autodoc-process-docstring', autodoc_remove_init_methods)
    app.connect('autodoc-skip-member', autodoc_skip_properties)

# Add any paths that contain templates here, relative to this directory.
templates_path = ['_templates']

# List of patterns, relative to source directory, that match files and
# directories to ignore when looking for source files.
# This pattern also affects html_static_path and html_extra_path.
exclude_patterns = []


# The name of the Pygments (syntax highlighting) style to use.
#pygments_style = "sphinx"

# If true, `todo` and `todoList` produce output, else they produce nothing.
todo_include_todos = False

# -- Options for HTML output -------------------------------------------------

# The theme to use for HTML and HTML Help pages.  See the documentation for
# a list of builtin themes.
#
html_theme = 'furo'

# Add any paths that contain custom static files (such as style sheets) here,
# relative to this directory. They are copied after the builtin static files,
# so a file named "default.css" will overwrite the builtin "default.css".
html_static_path = ['_static']
html_css_files = [
    "styles.css",
]


# Intersphinx mapping
# NOTE: if these are changed, then doc/intersphinx/update.sh
# must be changed accordingly to keep auto-updated mappings working
# intersphinx_mapping = {
#     'python': ('https://docs.python.org/3', (None, 'intersphinx/python-objects.inv')),
#     'numpy': ('https://numpy.org/doc/stable', (None, 'intersphinx/numpy-objects.inv')),
#     'matplotlib': ('https://matplotlib.org/stable', (None, 'intersphinx/matplotlib-objects.inv')),
#     'pandas': (
#         'https://pandas.pydata.org/pandas-docs/stable',
#         (None, 'intersphinx/pandas-objects.inv'),
#     ),
# }
