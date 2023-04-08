"""
IPython 3D Model Viewer
-------------
Render Vivid3D Objects in IPython
and jupyter notebooks using show
"""
import base64
import os
from ..utils import in_notebook
from vivid3d._core import BlobData

import tempfile
import webbrowser
from time import sleep


def view_glb(glb, height=600):
    """
    Convert a scene to HTML containing embedded geometry
    and a three.js viewer that will display nicely in
    an IPython/Jupyter notebook, or in web browser.
    
    Parameters
    ----------
    glb : bytes
        .glb encoded blob file
    height : number, default: 600
        height in pixels to open the viewer

    Returns
    -------
    html : string or IPython.display.HTML
      The HTML page

    """
    # convert scene to a full HTML page
    template_path = os.path.join(os.path.dirname(__file__), 'template.html')
    with open(template_path, 'r', encoding="utf-8") as f:
        template = f.read()
    # This increases file size by ~30%.
    encoded = base64.b64encode(glb).decode('utf-8')
    # replace keyword with our scene data
    srcdoc = template.replace('$B64GLTF', encoded)

    if in_notebook():  # Display in notebook
        # escape the quotes in the HTML
        srcdoc = srcdoc.replace('"', '&quot;')
        # keep as soft dependency
        from IPython import display
        # embed this puppy as the srcdoc attr of an IFframe
        return display.HTML('<div><iframe srcdoc="{srcdoc}" width="100%" height="{height}px" style="border:none;"></iframe></div>'.format(srcdoc=srcdoc, height=height))
    else:  # Attempt to display in browser
        # Make a temporary file that can be opened, will be deleted as soon as web-browser opens it
        with tempfile.NamedTemporaryFile('w', suffix='.html') as html:
            url = 'file://' + html.name
            html.write(srcdoc)
            webbrowser.open(url)
            sleep(3)


def show(model, height=600):
    """
    Convert a scene to HTML containing embedded geometry
    and a three.js viewer that will display nicely in
    an IPython/Jupyter notebook, or in web browser.

    Parameters
    ----------
    model : 
        vivid3d.Model or vivid3d.BaseMesh
    height : int, default: 600
        height in pixels to open the viewer

    Returns
    -------
    html : string
      The HTML page

    Notes
    -----
    The viewer works best in Chromium browsers. Firefox is limited to small models (<32Mb).
    Models larger than >100Mb may not load. If your model is too large, you can compress individual meshes using the .decimate() method.
    If you are experiencing further issues, we recommend exporting the model using .export() and dragging the file into the viewer window.
    """
    try:
        glb = model.export_to_blob(file_type="glb")

        if isinstance(glb, bytes):
            bytearray = glb
        elif isinstance(glb, BlobData):
            bytearray = glb.files[0]
        else:
            raise Exception("Model could not be parsed to proper format")
        embedded = view_glb(bytearray, height)
        return embedded
    except BaseException as err:
        print(err)
