import sys
import matplotlib.patches as patches
from . import box, barr

class ActionDict(dict):
    def __missing__(self, key):
        def do_nothing(): pass
        return do_nothing

action = ActionDict()



button_off   = patches.Circle((2.2, 2.2), 0.2, picker=1, color='orange')
action[button_off] = lambda: sys.exit(0)



button_reset = patches.Circle((1.0, 2.2), 0.2, picker=1)
def reset():
    box.reset()
    box.barriers = []
    box.redraw = True
action[button_reset] = reset

button_reset_A = patches.Circle((0.5, 2.2), 0.2, picker=1)
def reset_A():
    box.reset()
    box.barriers = barr
    box.redraw = True
action[button_reset_A] = reset_A




def key_press(event):
    print('press', event.key)
    sys.stdout.flush()
    if event.key == 'q':
        sys.exit(0)

def connect_buttons(axis, figure):

    axis.add_patch(button_off)
    axis.add_patch(button_reset)
    axis.add_patch(button_reset_A)

    figure.canvas.mpl_connect('key_press_event', key_press)
    figure.canvas.mpl_connect('pick_event', lambda ev: action[ev.artist]() )

