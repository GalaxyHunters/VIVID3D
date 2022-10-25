"""
IPython 2D 3-Axis Volumetric Viewer
-------------
"""

from matplotlib import animation
import matplotlib.pyplot as plt
import numpy as np
from .. import utils


# Todo: fix timesteps - 3 different animations trying to get them all to sync up?
class VolumetricViewer(object):
    """
    Utility Class for volumetric data pre-visualization. **IPython Required For View**

    This class serves as a simple way to visualize volumetric datasets pre-vivid,
    primarily for visualizing the mask for VoronoiVolume.mask_mesh

    Attributes
    ----------
    volume : 3D ndarray with single value per cell
      The volume to visualize. Very large datasets might not work well.

    Notes
    -----
    Many IPython environments require the following magic to properly display the Widget
    %matplotlib inline
    And the following magic to properly display the animation:
    %matplotlib notebook
    """

    def __init__(self, volume, cmap="viridis"):
        self.volume = volume
        self.__volume = volume
        self.cmap = cmap
        self.vmin = volume.min()
        self.vmax = volume.max()
        self.n_frames = max(volume.shape) - 1
        if utils.in_notebook():
            self.view()

    def view(self):
        """
        Creates GUI for volume visualization, masking, and colormap selection
        Runs on object creation.

        Notes
        -----
        Many IPython environments require the following magic to properly display the plots:
        %matplotlib inline
        """
        from ipywidgets import widgets, AppLayout, interactive_output
        from IPython.display import display

        title = widgets.HTML('<center><h1>Volumetric Viewer</h1></center>')
        label1 = widgets.Label(value="Thresholds")
        thresholds = widgets.FloatRangeSlider(value=(self.vmin, self.vmax), min=self.vmin, max=self.vmax,
                                              step=1 if self.vmax - self.vmin > 10 else (self.vmax - self.vmin) / 100.0,
                                              name="Mask Range")

        cm_dropdown = widgets.Dropdown(options=plt.colormaps(), value=self.cmap, description="Color Map",
                                       disable=False)
        label2 = widgets.Label(value="Frame Select")
        play = widgets.Play(
            value=self.n_frames // 2,
            min=0,
            max=self.n_frames,
            step=1,
            interval=250,
            description="Play Animation",
        )
        # fix missing icons in google colab
        display(widgets.HTML(
            '''<link rel="stylesheet" href="https://stackpath.bootstrapcdn.com/font-awesome/4.7.0/css/font-awesome.min.css"> '''))
        slider = widgets.IntSlider(value=self.n_frames // 2, min=0, max=self.n_frames, step=1)
        widgets.jslink((play, 'value'), (slider, 'value'))
        controls1 = widgets.VBox([title, widgets.HBox([label1, thresholds])],
                                 layout=widgets.Layout(display='flex', flex_flow='column', align_items='center'))
        controls2 = widgets.HBox([label2, play, slider, cm_dropdown],
                                 layout=widgets.Layout(display='flex', flex_flow='row', justify_content='center',
                                                       align_items='center'))
        out = interactive_output(self.__volumetric_mask_viewer, {
            'thresholds': thresholds,
            'frame': slider,
            'cmap': cm_dropdown
        })
        out.layout = widgets.Layout(width='95%')

        display(AppLayout(header=controls1, center=out, footer=controls2, width='50%'))

    def render_animation(self, fps=25, min_threshold=None, max_threshold=None, cmap=None):
        """
        Render the volume with the given mask into an animation.

        Parameters
        ----------
        fps : float, optional
            How many frames to animate per second. Default is 25.
        min_threshold : float, optional
            The minimum threshold for the mask.  If none specified, the value from the GUI will be used.
        max_threshold : float, optional
            The maximum threshold for the mask.  If none specified, the value from the GUI will be used.
        cmap : string, optional
            The matplotlib colormap to use. If none specified, the value from the GUI will be used.

        Returns
        -------
        animation : matplotlib.animation.Animation
            The Animation instance

        Notes
        -----
        Many IPython environments require the following magic to properly display the animation:
        %matplotlib notebook
        """
        if cmap: self.cmap = cmap
        fig, images = self.__base_plots()
        fig.suptitle('Volumetric Animation', fontsize=24)
        fig.tight_layout(pad=4)
        fig.set_tight_layout(False)
        self.__mask_volume(min_threshold, max_threshold)

        def animate(frame):
            for i, img in enumerate(images):
                orientation = self.__orientate_volume(i)
                orientation = orientation[min(orientation.shape[0] - 1, frame)]
                fig.axes[i].set_aspect(orientation.shape[0] / orientation.shape[1])
                fig.axes[i].set_xlim(0, orientation.shape[0])
                fig.axes[i].set_ylim(0, orientation.shape[1])
                img.set_extent([0, orientation.shape[0], 0, orientation.shape[1]])
                img.set_data(orientation)
            return images

        animation_3d = animation.FuncAnimation(fig, animate, self.n_frames, interval=1000 / fps, blit=False,
                                               cache_frame_data=False)

        return animation_3d

    def __volumetric_mask_viewer(self, thresholds, frame, cmap):
        self.cmap = cmap
        fig, images = self.__base_plots()

        self.__mask_volume(thresholds[0], thresholds[1])
        for i, img in enumerate(images):
            orientation = self.__orientate_volume(i)
            orientation = orientation[min(orientation.shape[0] - 1, frame)]
            fig.axes[i].set_aspect(orientation.shape[0] / orientation.shape[1])
            fig.axes[i].set_xlim(0, orientation.shape[0])
            fig.axes[i].set_ylim(0, orientation.shape[1])
            img.set_extent([0, orientation.shape[0], 0, orientation.shape[1]])
            img.set_data(orientation)
        return fig

    def __base_plots(self):
        fig, axes = plt.subplots(1, 4, False, False, figsize=(18, 6), gridspec_kw={'width_ratios': [1, 1, 1, .07], })
        # fig.tight_layout(pad=1)
        plt.set_cmap(self.cmap)

        images = [ax.imshow(np.empty([2, 2]), vmin=self.__volume.min(), vmax=self.__volume.max(), aspect='equal',
                            cmap=self.cmap) for i, ax in enumerate(fig.axes[:-1])]
        plt.xticks([])
        plt.yticks([])
        ft = 16
        # for ax in axes[:-1]:
        #   ax.tick_params(left = False, right = False , labelleft = False ,
        #             labelbottom = False, bottom = False)
        axes[0].set_xlabel("X Axis", fontsize=ft), axes[0].set_ylabel("Y Axis", fontsize=ft)
        axes[1].set_xlabel("Y Axis", fontsize=ft), axes[1].set_ylabel("Z Axis", fontsize=ft)
        axes[2].set_xlabel("Z Axis", fontsize=ft), axes[2].set_ylabel("X Axis", fontsize=ft)

        fig.colorbar(images[2], cax=axes[-1])
        axes[3].set_aspect(20)

        return fig, images

    def __orientate_volume(self, i):
        if i == 0:
            return self.__volume
        elif i == 1:
            return np.transpose(self.__volume, (1, 2, 0))
        elif i == 2:
            return np.transpose(self.__volume, (2, 1, 0))
        else:
            raise IndexError(f"{i} is not in range")

    def __mask_volume(self, min_threshold, max_threshold):
        if min_threshold is None:
            self.__volume = self.volume
            return
        if max_threshold is None:
            max_threshold = self.volume.max()

        is_masked = max_threshold != self.volume.max() or min_threshold != self.volume.min()
        if is_masked:
            self.__volume = np.where((self.volume <= max_threshold) & (self.volume >= min_threshold), 1, 0)
        else:
            self.__volume = self.volume

# Example
# X, Y, Z = np.mgrid[-1:1:30j, -1:1:30j, -1:1:30j]
# volume  = np.sin(np.pi*X) * np.cos(np.pi*Z) * np.sin(np.pi*Y)
# from skimage import io
# mri = np.rot90(io.imread("https://s3.amazonaws.com/assets.datacamp.com/blog_assets/attention-mri.tif"), -1, axes=(2,1))
# 
# viewer = VolumetricViewer(volume)
# viewer.render_animation()
