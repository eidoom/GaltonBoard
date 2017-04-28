import numpy as np
import matplotlib.pyplot as plt
from matplotlib.gridspec import GridSpec


gs = GridSpec(2, 1,
              #width_ratios=[1,2],
              height_ratios=[4,1]
              )


fig = plt.figure()
fig.subplots_adjust(hspace=0)

ax_live  = fig.add_subplot(gs[0])
plt.setp(ax_live.get_yticklabels() + ax_live.get_yticklines(),
         visible=False)

ax_histo = fig.add_subplot(gs[1])

#(fig, (ax_live, ax_histo)) = plt.subplots(2,sharex=True)


ax_live.plot([1,2,3])

ax_histo.hist([3,2,1])

plt.show()
