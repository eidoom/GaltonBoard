from . import BOXSIZE
import numpy as np
from scipy.spatial.distance import cdist # pdist, squareform, cdist


# Animation of Elastic collisions with Gravity
# 
# author: Jake Vanderplas
# email: vanderplas@astro.washington.edu
# website: http://jakevdp.github.com
# license: BSD
# Please feel free to use and modify this, but keep the above information. Thanks!

NN = 300

class ParticleBox(object):
    """Orbits class
    
    init_state is an [N x 4] array, where N is the number of particles:
       [[x1, y1, vx1, vy1],
        [x2, y2, vx2, vy2],
        ...               ]

    bounds is the size of the box: [xmin, xmax, ymin, ymax]
    """
    def __init__(self,
                 fixed_grid = [[-0.9, 0.9],
                               [-0.3, 0.3]], 
                 barriers = None,
                 bounds = [-BOXSIZE, BOXSIZE, -BOXSIZE, BOXSIZE],
                 size = 0.05,
                 M = 0.99,
                 G = 9.8):

        self.fixed_grid = np.asarray(fixed_grid, dtype=float)

        self.barriers = barriers

        self.reset()

        self.M          = np.full(self.state.shape[0], M, dtype=float)
        self.size = size
        self.time_elapsed = 0
        self.bounds = bounds
        self.G = G

        self.redraw = False

        self.endpos = []

        self.PAIRWISE = False
        self.FIXEDGRID = True
        self.GRAVITY = True
        self.FRICTION = True

        self.DAMPING = 1


    def reset(self):
        #np.random.seed(0)
        state = -0.5 + np.random.random((NN, 4))
        state[:, :2] *= 0.05
        state[:, 1] += BOXSIZE - 1.4
        ## zero initial velocity
        #state[:, 2:] = 0.0

        self.state  = state
        self.dead   = np.full(self.state.shape[0], False, dtype=bool)
        self.PAUSED  = False


    def step(self, dt):
        """step once by dt seconds"""
        self.time_elapsed += dt
        
        # update positions
        self.state[:, :2] += dt * self.state[:, 2:]

        if self.PAIRWISE: # pairwise interactions
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

        if self.FIXEDGRID: # fixed grid interactions
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
                self.state[i1, 2:] = self.DAMPING * v_new
        
        for bar in self.barriers: # bounces of barriers
            idx, = np.where(bar.check(self.state[:,:2]))
            for i in idx:
                if self.dead[i]: 
                    continue

                v = self.state[i,2:]
                r = bar.norm(self.state[i,0])

                rr = np.dot(r,r)
                vr = np.dot(r,v)

                new_v = v - r * (2 * vr / rr)

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
        if self.GRAVITY:
            self.state[:, 3] -= self.M * self.G * dt

        if self.FRICTION:
            self.state[:, 2:] *= 0.99

    def done(self):
        return np.all(self.dead)

    def registered_pos(self):
        return self.state[self.dead, 0]

