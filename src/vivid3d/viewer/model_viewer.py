"""
IPython 3D Model Viewer
-------------
Render Vivid3D Objects in IPython
and jupyter notebooks using show
"""
import base64
import os
from ..utils import in_notebook
from vivid3d._vivid import BlobData

import tempfile
import webbrowser
import time


def view_glb(glb):
    """
    Convert a scene to HTML containing embedded geometry
    and a three.js viewer that will display nicely in
    an IPython/Jupyter notebook, or in web browser.
    
    Parameters
    -------------
    glb : bytes
      .glb encoded blob file
      
    Returns
    -------------
    html : string
      The HTML page
    """
    # convert scene to a full HTML page
    template_path = os.path.join(os.path.dirname(__file__), 'template.html')
    with open(template_path, 'r', encoding="utf-8") as f:
        template = f.read()
    encoded = base64.b64encode(glb).decode('utf-8')
    # replace keyword with our scene data
    srcdoc = template.replace('$B64GLTF', encoded)

    if in_notebook():  # Display in notebook
        # escape the quotes in the HTML
        srcdoc = srcdoc.replace('"', '&quot;')
        # keep as soft dependency
        from IPython import display
        # embed this puppy as the srcdoc attr of an IFframe
        display.HTML(
            '<iframe srcdoc="{srcdoc}" '
            'width="100%" height="500px" '
            'style="border:none;"></iframe> '.format(srcdoc=srcdoc))
    else:  # Attempt to display in browser
        # Make a temporary file that can be opened, will be deleted as soon as web-browser opens it
        with tempfile.NamedTemporaryFile('w', suffix='.html') as html:
            url = 'file://' + html.name
            html.write(srcdoc)
            webbrowser.open(url)

    return srcdoc


def show(model):
    """
    Convert a scene to HTML containing embedded geometry
    and a three.js viewer that will display nicely in
    an IPython/Jupyter notebook, or in web browser.

    Parameters
    -------------
    model : vivid3d.Model or vivid3d.BaseMesh
      .glb encoded blob file

    Returns
    -------------
    html : string
      The HTML page
    """
    try:
        glb = model.export(file_type="glb")

        if isinstance(glb, bytes):
            bytearray = glb
        elif isinstance(glb, BlobData):
            bytearray = glb.files[0]
        else:
            raise Exception("Model could not be parsed to proper format")
        embedded = view_glb(bytearray)
        return embedded
    except BaseException as err:
        print(err)
