import sys
import matplotlib.patches as patches
from . import box

class ActionDict(dict):
    def __missing__(self, key):
        def do_nothing(): pass
        return do_nothing

action = ActionDict()

button_off   = patches.Circle((2.2, 2.2), 0.2, picker=1)
action[button_off] = lambda: sys.exit(0)

button_reset = patches.Circle((1.8, 2.2), 0.2, picker=1)
def reset():
    box.reset()
action[button_reset] = reset

def key_press(event):
    print('press', event.key)
    sys.stdout.flush()
    if event.key == 'q':
        sys.exit(0)

def connect_buttons(axis, figure):

    axis.add_patch(button_off)
    axis.add_patch(button_reset)

    figure.canvas.mpl_connect('key_press_event', key_press)
    figure.canvas.mpl_connect('pick_event', lambda ev: action[ev.artist]() )

