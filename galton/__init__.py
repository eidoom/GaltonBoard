import numpy as np
from barrier import barr
del barrier

BOXSIZE = 2.75
NN = 250

from galtonbox import ParticleBox

np.random.seed(0)
init_state = -0.5 + np.random.random((NN, 4))
init_state[:, :2] *= 0.05
init_state[:, 1] += BOXSIZE - 1.5

# zero initial velocity
#init_state[:, 2:] = 0.0


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

box = ParticleBox(init_state, fixed_grid, barr, size=0.04)


del fixed_grid
del init_state

del galtonbox
