"""
Define all interactive buttons and their actions
"""
import sys
import matplotlib.patches as patches
from . import box, preset_barriers
import numpy as np

class ActionDict(dict):
    """Map from pickable patch to function. Default does nothing."""
    def __missing__(self, key):
        def do_nothing(): pass
        return do_nothing

action = ActionDict()



button_off   = patches.Circle((2.2, 2.7), 0.4, picker=1, color='orange')
action[button_off] = lambda: sys.exit(0)



button_reset = patches.Wedge((0.0, 2.7), 0.4, 0, 360, width=0.05, picker=1)
def reset():
    box.reset()
    box.barriers = preset_barriers['empty']
    box.redraw = True
action[button_reset] = reset



button_reset_triangle = patches.RegularPolygon((-2.2, 2.45), 3, 0.5, picker=1)
def reset_triangle():
    box.reset()
    box.barriers = preset_barriers['triangle']
    box.redraw = True
action[button_reset_triangle] = reset_triangle


button_reset_funnel = patches.RegularPolygon((-1.1, 2.65), 3, 0.5, orientation = np.pi, picker=1)
def reset_funnel():
    box.reset()
    box.barriers = preset_barriers['funnel']
    box.redraw = True
action[button_reset_funnel] = reset_funnel




button_reset_left = patches.Arrow(1.25, 2.8, -0.6, -0.6, 1.0, picker=1)
def reset_left():
    box.reset()
    box.barriers = preset_barriers['left']
    box.redraw = True
action[button_reset_left] = reset_left


button_reset_right = patches.Arrow(2.0, 2.8, 0.6, -0.6, 1.0, picker=1)
def reset_right():
    box.reset()
    box.barriers = preset_barriers['right']
    box.redraw = True
action[button_reset_right] = reset_right



def key_press(event):
    if event.key == 'q':
        sys.exit(0)

def connect_buttons(axis, figure):
    """Display buttons on the canvas and connect up functionality"""
    #axis.add_patch(button_off)
    axis.add_patch(button_reset)
    axis.add_patch(button_reset_triangle)
    axis.add_patch(button_reset_funnel)
    axis.add_patch(button_reset_left)
    axis.add_patch(button_reset_right)

    figure.canvas.mpl_connect('key_press_event', key_press)
    figure.canvas.mpl_connect('pick_event', lambda ev: action[ev.artist]() )

