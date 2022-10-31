import os


def in_notebook():
    """
    Check to see if we are in an IPython or Jypyter notebook.
    Returns
    -----------
    in_notebook : bool
      Returns True if we are in a notebook
    """
    try:
        # function returns IPython context, but only in IPython
        ipy = get_ipython()  # NOQA
        # we only want to render rich output in notebooks
        # in terminals we definitely do not want to output HTML
        name = str(ipy.__class__).lower()
        terminal = 'terminal' in name

        # spyder uses ZMQshell, and can appear to be a notebook
        spyder = '_' in os.environ and 'spyder' in os.environ['_']

        # assume we are in a notebook if we are not in
        # a terminal and we haven't been run by spyder
        notebook = (not terminal) and (not spyder)

        return notebook

    except BaseException:
        return False
