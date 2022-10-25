"""
IPython 3D Model Viewer
-------------
Render Vivid3D Objects in IPython
and jupyter notebooks using show
"""
import base64


def view_glb(glb):
    """
    Convert a scene to HTML containing embedded geometry
    and a three.js viewer that will display nicely in
    an IPython/Jupyter notebook.
    
    Parameters
    -------------
    glb : binary
      .glb blob file
      
    Returns
    -------------
    html : IPython.display.HTML
      Object containing rendered scene
    """
    # keep as soft dependency
    from IPython import display

    # convert scene to a full HTML page
    # fetch HTML template
    with open('template.html', 'r', encoding="utf8") as f:
        template = f.read()
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
