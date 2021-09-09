"""
html model generator
-------------
Render gltf objects in HTML
and jupyter notebooks using model-viewer
"""
import os
import base64
import io

def encode_html(path):
    """
    Return HTML that will render the scene using
    GLTF/GLB encoded to base64 loaded by model-viewer
    Parameters
    --------------
    model: .glb file
    #TODO test with gltf
    Returns
    --------------
    html : str
      HTML containing embedded geometry
    """
    # fetch HTML template
    with open('template.html', 'r', encoding="utf8") as f:
        base = f.read()
    # get export as bytes
    with open(path,'rb') as model:
        data = model.read()
    # encode as base64 string
    encoded = "data:image/jpeg;base64,{model}".format(model=base64.b64encode(data).decode('utf-8'))
    # replace keyword with our scene data
    result = base.replace('$B64GLTF', encoded) # $B64GLTF has no meaning just a placeholder in the template file
    return result

def show(path, height=500, **kwargs):
    """
    Convert a scene to HTML containing embedded geometry
    and a three.js viewer that will display nicely in
    an IPython/Jupyter notebook.
    Parameters
    -------------
    scene : trimesh.Scene
      Source geometry
    Returns
    -------------
    html : IPython.display.HTML
      Object containing rendered scene
    """
    # keep as soft dependency
    from IPython import display

    # convert scene to a full HTML page
    as_html = encode_html(path)

    # escape the quotes in the HTML
    srcdoc = as_html.replace('"', '&quot;')

    # embed this puppy as the srcdoc attr of an IFframe
    embedded = display.HTML(
        '<iframe srcdoc="{srcdoc}" '
        'width="100%" height="{height}px" '
        'style="border:none;"></iframe> '.format(
            srcdoc=srcdoc,
            height=height))
    return embedded



if __name__ == "__main__":
    show('model.glb')