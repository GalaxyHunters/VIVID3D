"""
html model generator
-------------
Render gltf objects in HTML
and jupyter notebooks using model-viewer
"""
import os
import base64

def encode_html(glb):
    """
    Return HTML that will render the scene using
    GLTF/GLB encoded to base64 loaded by model-viewer
    #TODO run in python 2.7
    #TODO run with
    Parameters
    --------------
    :param glb: glb file binary

    Returns
    --------------
    html : str
      HTML containing embedded geometry

    """
    # fetch HTML template
    with open('template.html', 'r', encoding="utf8") as f:
        base = f.read()
    # get export as bytes
    # with open(path,'rb') as model:
    #    data = model.read()
    data = glb
    # encode as base64 string
    encoded = "data:image/jpeg;base64,{model}".format(model=base64.b64encode(data).decode('utf-8'))
    # replace keyword with our scene data
    result = base.replace('$B64GLTF', encoded) # $B64GLTF has no meaning just a placeholder in the template file
    return result


def show(path, height=500, **kwargs):
    """
    Convert a glb to HTML containing embedded geometry
    and a module viewer instance that will display nicely in
    an IPython/Jupyter notebook.
    Parameters
    -------------
    Srt : path to the glb file
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