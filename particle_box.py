"""
Animation of Elastic collisions with Gravity

author: Jake Vanderplas
email: vanderplas@astro.washington.edu
website: http://jakevdp.github.com
license: BSD
Please feel free to use and modify this, but keep the above information. Thanks!
"""
import numpy as np

import matplotlib as mpl
mpl.rcParams['toolbar'] = 'None'

from scipy.spatial.distance import pdist, squareform, cdist

import matplotlib.pyplot as plt
import scipy.integrate as integrate
import matplotlib.animation as animation

PAIRWISE = False
FIXEDGRID = True
GRAVITY = True

DAMPING = 1.0

BOXSIZE = 2.75

PAUSED = False

class ParticleBox:
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
                 bounds = [-BOXSIZE, BOXSIZE, -BOXSIZE, BOXSIZE],
                 size = 0.04,
                 M = 0.1,
                 G = 9.8):
        self.state      = np.asarray(init_state, dtype=float)
        self.M          = np.full(self.state.shape[0], M, dtype=float)
        self.dead       = np.full(self.state.shape[0], False, dtype=bool)

        self.fixed_grid = np.asarray(fixed_grid, dtype=float)
        self.size = size
        self.time_elapsed = 0
        self.bounds = bounds
        self.G = G

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
            ind1, ind2 = np.where(D < 2 * self.size)

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

    def done(self):
        return np.all(self.dead)


#------------------------------------------------------------
# set up initial state
np.random.seed(0)
init_state = -0.5 + np.random.random((20, 4))
init_state[:, :2] *= 0.5
init_state[:, 1] += BOXSIZE - 0.5

# zero initial velocity
init_state[:, 2:] = 0.0


triangle_height = np.sqrt(0.75)
fixed_grid = []
for i,y in enumerate(np.linspace(-0.6*BOXSIZE, 0.75*BOXSIZE, 7)):
    for x in np.linspace(-0.1*BOXSIZE*i, 0.1*BOXSIZE*i, i+1):
        fixed_grid.append([x, -triangle_height*y])

fixed_grid = np.asarray(fixed_grid, dtype=float)
fixed_grid *= 0.8
#fixed_grid[:,1] += 1

# fixed_grid = -0.5 + np.random.random((10,2))
# fixed_grid *= 3.5

box = ParticleBox(init_state, fixed_grid, size=0.04)
dt = 1. / 30 # 30fps


#------------------------------------------------------------
# set up figure and animation
fig = plt.figure()
fig.subplots_adjust(left=0, right=1, bottom=0, top=1)
ax = fig.add_subplot(111, aspect='equal', autoscale_on=False,
                     xlim=(-1.6*BOXSIZE, 1.6*BOXSIZE), ylim=(-1.2*BOXSIZE, 1.2*BOXSIZE))




ms = int(fig.dpi * 2 * box.size * fig.get_figwidth()
         / np.diff(ax.get_xbound())[0])

# particles holds the locations of the particles
particles, = ax.plot([], [], 'bo', ms=ms)
fixed, = ax.plot(box.fixed_grid[:, 0], box.fixed_grid[:, 1], 'ro', ms=ms)

# rect is the box edge
rect = plt.Rectangle(box.bounds[::2],
                     box.bounds[1] - box.bounds[0],
                     box.bounds[3] - box.bounds[2],
                     ec='k', lw=2, fc='none')
ax.add_patch(rect)

def init():
    """initialize animation"""
#    global box, rect
#    particles.set_data([], [])
#    rect.set_edgecolor('k')
    return particles, rect

def animate(i):
    """perform animation step"""
    global box, rect, dt, ax, fig, PAUSED
    if not PAUSED:
        box.step(dt)
        print box.done()
        if box.done():
            PAUSED = True
    # update pieces of the animation
    # rect.set_edgecolor('k')
    # alive = box.state[box.dead_pts]
        particles.set_data(box.state[~box.dead][:, 0], 
                           box.state[~box.dead][:, 1])
    return particles, rect

# need to hold the handle to avoid GC
ani = animation.FuncAnimation(fig, animate, #frames=600,
                              interval=60, blit=True, init_func=init)



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
