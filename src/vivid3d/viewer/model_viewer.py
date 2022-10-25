"""
IPython 3D Model Viewer
-------------
Render Vivid3D Objects in IPython
and jupyter notebooks using show
"""
import base64
import os
from vivid3d._vivid import BlobData

def view_glb(glb):
    """
    Convert a scene to HTML containing embedded geometry
    and a three.js viewer that will display nicely in
    an IPython/Jupyter notebook.
    
    Parameters
    -------------
    glb : bytes
      .glb encoded blob file
      
    Returns
    -------------
    html : IPython.display.HTML
      Object containing rendered scene
    """
    # keep as soft dependency

    # convert scene to a full HTML page
    # fetch HTML template
    template_path = os.path.join(os.path.dirname(__file__), 'template.html')
    with open(template_path, 'r', encoding="utf8") as f:
        template = f.read()
    from IPython import display
    # get export as bytes
    # with open(path,'rb') as model:
    #     data = model.read()
    # encode as base64 string
    encoded = base64.b64encode(glb).decode('utf-8')
    # replace keyword with our scene data
    as_html = template.replace('$B64GLTF', encoded)  # $B64GLTF has no meaning just a placeholder in the template file

    # escape the quotes in the HTML
    srcdoc = as_html.replace('"', '&quot;')

    # embed this puppy as the srcdoc attr of an IFframe
    embedded = display.HTML(
        '<iframe srcdoc="{srcdoc}" '
        'width="100%" height="500px" '
        'style="border:none;"></iframe> '.format(srcdoc=srcdoc))
    return embedded


def show(model):
    # Works with trimesh and vivid models
    try:
        glb = model.export(file_type="glb")

        if isinstance(glb, bytes):
            bytearray = glb
        elif isinstance(glb, BlobData):
            bytearray = glb.files[0]
        else:
            raise Exception("")
        embedded = view_glb(bytearray)
        return embedded
    except Exception:
        print("Model could not be parsed to proper format")
