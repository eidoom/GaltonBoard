"""
Animation of Elastic collisions with Gravity

author: Jake Vanderplas
email: vanderplas@astro.washington.edu
website: http://jakevdp.github.com
license: BSD
Please feel free to use and modify this, but keep the above information. Thanks!
"""
import sys
import numpy as np

import matplotlib as mpl
mpl.rcParams['toolbar'] = 'None'

from scipy.spatial.distance import pdist, squareform, cdist

import matplotlib.pyplot as plt

import matplotlib.gridspec as gridspec

import matplotlib.patches as patches
import matplotlib.path as path
import matplotlib.animation as animation

from matplotlib.widgets import Button


import scipy.integrate as integrate
import matplotlib.animation as animation

PAIRWISE = False
FIXEDGRID = True
GRAVITY = True
FRICTION = True
BARRIER = True

DAMPING = 1

BOXSIZE = 2.75

PAUSED = False



class Barrier(object):
    def __init__(self, min_x, max_x, fn):
        self.min = min_x
        self.max = max_x

        self.fun = np.vectorize(fn)
        # self.fun = lambda x: np.piecewise(x, 
        #                        [x < self.min, (x >= self.min) & (x < self.max), x >= self.max],
        #                        [np.inf, fn, np.inf]
        #            )

        self.norm = np.array([-1,1])

    def check(self, x):
        """Return bool array selecting the Nx2 grid elements that touch the line."""
        return (
            (x[:,0] > self.min) 
            & (x[:,0] < self.max) 
            & (np.abs(self.fun(x[:,0]) - x[:,1]) < 0.1)
        )

    @property
    def xs(self):
        return np.linspace(self.min, self.max, 30)

    @property
    def ys(self):
        return self.fun(self.xs)


# barrier = Barrier()

# x = np.random.uniform(-1,1,size=(20,2))
# print x

# print barrier.fun(x[:,0])


#exit(0)

class ParticleBox(object):
    """Orbits class
    
    init_state is an [N x 4] array, where N is the number of particles:
       [[x1, y1, vx1, vy1],
        [x2, y2, vx2, vy2],
        ...               ]

    bounds is the size of the box: [xmin, xmax, ymin, ymax]
    """
    def __init__(self,
                 init_state = [[1, 0, 0, -1],
                               [-0.5, 0.5, 0.5, 0.5],
                               [-0.5, -0.5, -0.5, 0.5]],
                 fixed_grid = [[-0.9, 0.9],
                               [-0.3, 0.3]], 
                 barrier = None,
                 bounds = [-BOXSIZE, BOXSIZE, -BOXSIZE, BOXSIZE],
                 size = 0.04,
                 M = 0.99,
                 G = 9.8):
        self.state      = np.asarray(init_state, dtype=float)
        self.M          = np.full(self.state.shape[0], M, dtype=float)
        self.dead       = np.full(self.state.shape[0], False, dtype=bool)

        self.fixed_grid = np.asarray(fixed_grid, dtype=float)

        self.barrier = barrier

        self.size = size
        self.time_elapsed = 0
        self.bounds = bounds
        self.G = G

        self.endpos = []

    def step(self, dt):
        """step once by dt seconds"""
        self.time_elapsed += dt
        
        # update positions
        self.state[:, :2] += dt * self.state[:, 2:]

        if PAIRWISE: # pairwise interactions
            # find pairs of particles undergoing a collision
            D = squareform(pdist(self.state[:, :2]))
            ind1, ind2 = np.where(D < 2 * self.size)
            unique = (ind1 < ind2)
            ind1 = ind1[unique]
            ind2 = ind2[unique]

            # update velocities of colliding pairs
            for i1, i2 in zip(ind1, ind2):

                if self.dead[i1] or self.dead[i2]:
                    continue

                # mass
                m1 = self.M[i1]
                m2 = self.M[i2]

                # location vector
                r1 = self.state[i1, :2]
                r2 = self.state[i2, :2]

                # velocity vector
                v1 = self.state[i1, 2:]
                v2 = self.state[i2, 2:]

                # relative location & velocity vectors
                r_rel = r1 - r2
                v_rel = v1 - v2

                # momentum vector of the center of mass
                v_cm = (m1 * v1 + m2 * v2) / (m1 + m2)

                # collisions of spheres reflect v_rel over r_rel
                rr_rel = np.dot(r_rel, r_rel)
                vr_rel = np.dot(v_rel, r_rel)
                v_rel = 2 * r_rel * vr_rel / rr_rel - v_rel

                # assign new velocities
                self.state[i1, 2:] = v_cm + v_rel * m2 / (m1 + m2)
                self.state[i2, 2:] = v_cm - v_rel * m1 / (m1 + m2) 

        if FIXEDGRID: # fixed grid interactions
            # find pairs of particles undergoing a collision
            D = cdist(self.state[:, :2], self.fixed_grid)
            ind1, ind2 = np.where(D < 1.7 * self.size)

            # update velocities of colliding pairs
            for i1, i2 in zip(ind1, ind2):
                if self.dead[i1]:
                    continue

                # location vector
                r1 = self.state[i1, :2]
                r2 = self.fixed_grid[i2]

                # velocity vector
                v = self.state[i1, 2:]

                # relative location
                r_rel = r1 - r2

                # collisions of spheres reflect v_rel over r_rel
                rr_rel = np.dot(r_rel, r_rel)
                vr_rel = np.dot(v,     r_rel)
                v_new  = v - r_rel * (2 * vr_rel / rr_rel)

                # assign new velocities
                self.state[i1, 2:] = DAMPING * v_new
        
        if BARRIER and self.barrier: # bounces of barriers
            idx, = np.where(self.barrier.check(self.state[:,:2]))
            for i in idx:
                if self.dead[i]: 
                    continue

                v = self.state[i,2:]
                r = self.barrier.norm

                rr = np.dot(r,r)
                rv = np.dot(r,v)

                new_v = -2 * r * rv / rr + v

                self.state[i,2:] = new_v
              


        # check for crossing boundary
        crossed_x1 = (self.state[:, 0] < self.bounds[0] + self.size)
        crossed_x2 = (self.state[:, 0] > self.bounds[1] - self.size)
        crossed_y1 = (self.state[:, 1] < self.bounds[2] + self.size)
        crossed_y2 = (self.state[:, 1] > self.bounds[3] - self.size)

        self.state[crossed_x1, 0] = self.bounds[0] + self.size
        self.state[crossed_x2, 0] = self.bounds[1] - self.size

        self.state[crossed_y1, 1] = self.bounds[2] + self.size
        self.state[crossed_y2, 1] = self.bounds[3] - self.size

        # reflect velocities at wall
        self.state[crossed_x1 | crossed_x2, 2] *= -1
        self.state[crossed_y1 | crossed_y2, 3] *= -1

        self.state[crossed_y1, 2:4] = 0 
        self.dead[crossed_y1] = True

        # add gravity
        if GRAVITY:
            self.state[:, 3] -= self.M * self.G * dt

        if FRICTION:
            self.state[:, 2:] *= 0.98

    def done(self):
        return np.all(self.dead)

    def registered_pos(self):
        return self.state[self.dead, 0]


#------------------------------------------------------------
# set up initial state

NN = 50

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

fixed_grid = np.asarray(fixed_grid, dtype=float)
fixed_grid *= 0.7
fixed_grid[:,1] -= 0.5

# fixed_grid = -0.5 + np.random.random((10,2))
# fixed_grid *= 3.5

barr = Barrier(-2, -0.2, lambda x: 1.45*x + 1.45)

box = ParticleBox(init_state, fixed_grid, barr, size=0.04)
dt = 1. / 100 # 30fps


#------------------------------------------------------------
# set up figure and animation
fig = plt.figure(figsize=(8,10))
#fig.subplots_adjust(left=0, right=1, bottom=0, top=1)
#ax = fig.add_subplot(211, aspect='equal', autoscale_on=False,
#                     xlim=(-1.6*BOXSIZE, 1.6*BOXSIZE), ylim=(-1.2*BOXSIZE, 1.2*BOXSIZE))

#ax_h = fig.add_subplot(212, aspect='equal', autoscale_on=False,
#                     xlim=(-1.6*BOXSIZE, 1.6*BOXSIZE), ylim=(-1.2*BOXSIZE, 1.2*BOXSIZE))

gs = gridspec.GridSpec(2, 1, height_ratios=[4, 1], wspace=0.0, hspace=0.0)

ax_h = plt.subplot(gs[1])

n, bins = np.histogram([], bins=13, range=(-BOXSIZE,BOXSIZE))
left = np.array(bins[:-1])
right = np.array(bins[1:])
bottom = np.zeros(len(left))
top = bottom + n
nrects = len(left)

nverts = nrects*(1 + 3 + 1)
verts = np.zeros((nverts, 2))
codes = np.ones(nverts, int) * path.Path.LINETO
codes[0::5] = path.Path.MOVETO
codes[4::5] = path.Path.CLOSEPOLY
verts[0::5, 0] = left
verts[0::5, 1] = bottom
verts[1::5, 0] = left
verts[1::5, 1] = top
verts[2::5, 0] = right
verts[2::5, 1] = top
verts[3::5, 0] = right
verts[3::5, 1] = bottom

barpath = path.Path(verts, codes)
patch = patches.PathPatch(
    barpath, facecolor='green', edgecolor='yellow', alpha=0.5)

ax_h.add_patch(patch)
ax_h.set_xlim(left[0], right[-1])
ax_h.set_ylim((0, NN/5.))
ax_h.set_xticks([])


ax = plt.subplot(gs[0],
        aspect='equal', 
        autoscale_on=False,
        xlim=(-BOXSIZE, BOXSIZE), 
        ylim=(-BOXSIZE, BOXSIZE),
        sharex=ax_h
     )
#ax.set_xticks([])
#ax.set_yticks([])




ms = int(fig.dpi * 2 * box.size * fig.get_figwidth()
         / np.diff(ax.get_xbound())[0])

# particles holds the locations of the particles
particles, = ax.plot([], [], 'bo', ms=ms)

if FIXEDGRID:
    fixed, = ax.plot(box.fixed_grid[:, 0], box.fixed_grid[:, 1], 'ro', ms=ms)

if BARRIER:
    barriers, = ax.plot(barr.xs, barr.ys)

histo = ax_h.hist([],bins=13)



def init():
    """initialize animation"""
#    global box, rect
#    particles.set_data([], [])
#    rect.set_edgecolor('k')
    return particles, patch

def animate(i):
    """perform animation step"""
    global box, rect, dt, ax, fig, PAUSED
    if not PAUSED:
        box.step(dt)

        if box.done():
            PAUSED = True
        # update pieces of the animation
        # rect.set_edgecolor('k')
        # alive = box.state[box.dead_pts]
        particles.set_data(box.state[~box.dead][:, 0], 
                           box.state[~box.dead][:, 1])

        data = box.registered_pos()
        n, bins = np.histogram(data, bins=13, range=(-BOXSIZE,BOXSIZE))
        top = bottom + n
        verts[1::5, 1] = top
        verts[2::5, 1] = top

    return particles, patch

# need to hold the handle to avoid GC
ani = animation.FuncAnimation(fig, animate, #frames=600,
                              interval=40, blit=True, init_func=init)


button_off = patches.Circle((2.2, 2.2), 0.2, picker=1)
ax.add_patch(button_off)


def key_press(event):
    print('press', event.key)
    sys.stdout.flush()
    if event.key == 'q':
        sys.exit(0)
        # visible = xl.get_visible()
        # xl.set_visible(not visible)
        # fig.canvas.draw()

def pick(event):
    artist = event.artist

    print dir(event)

    print artist
    print artist == button_off

    if artist == button_off:
        sys.exit(0)


fig.canvas.mpl_connect('key_press_event', key_press)
fig.canvas.mpl_connect('pick_event', pick)


# def onDraw(event):
#     global PAUSED
#     if box.done():
#         PAUSED = True
# #        ani.event_source.stop()


# fig.canvas.mpl_connect('draw_event', onDraw)


# save the animation as an mp4.  This requires ffmpeg or mencoder to be
# installed.  The extra_args ensure that the x264 codec is used, so that
# the video can be embedded in html5.  You may need to adjust this for
# your system: for more information, see
# http://matplotlib.sourceforge.net/api/animation_api.html
#ani.save('particle_box.mp4', fps=30, extra_args=['-vcodec', 'libx264'])

plt.show()
