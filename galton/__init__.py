import numpy as np
from barrier import preset_barriers
del barrier

BOXSIZE = 2.75

from galtonbox import ParticleBox

triangle_height = np.sqrt(0.75)
fixed_grid = []
for i,y in enumerate(np.linspace(-BOXSIZE, BOXSIZE, 13)):
    for x in np.linspace(-0.1*BOXSIZE*i, 0.1*BOXSIZE*i, i+1):
        fixed_grid.append([x, -triangle_height*y])
del triangle_height

fixed_grid = np.asarray(fixed_grid, dtype=float)
fixed_grid *= 0.7
fixed_grid[:,1] -= 0.5

# fixed_grid = -0.5 + np.random.random((10,2))
# fixed_grid *= 3.5

box = ParticleBox(fixed_grid, preset_barriers['empty'], size=0.04)

del fixed_grid

del galtonbox
