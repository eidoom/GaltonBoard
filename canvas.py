import pygame
import sys
import random
import numpy as np

pygame.init()

width, height = 800, 800
screen = pygame.display.set_mode((width,height))
pygame.display.set_caption('Tutorial 1')

background_colour = (255,255,255)
screen.fill(background_colour)

G = np.array([0,9.81e-2],dtype=np.float64)
DRAG = 1.0 #0.99
BOUNCE_LOSS = 1#0.9

class Particle(object):
    def __init__(self,(x,y),size,colour=(0,0,155)):
        self.x = np.array([x,y],dtype=np.float64)
        self.size = size
        self.colour = colour
        self.thickness = 1
        self.v = np.zeros(2,dtype=np.float64)
        self.frozen = False

    @property
    def T(self):
        return 0.5* (self.v.dot(self.v))

    @property
    def U(self):
        return G[1]*height - G.dot(self.x)

    @property
    def E(self):
        return self.T + self.U

    def display(self):
        pygame.draw.circle(screen, 
                           self.colour,
                           (int(self.x[0]), int(self.x[1])),
                           self.size,
                           self.thickness
                           )

    def freeze(self):
        return
        self.v *= 0
        self.frozen = True

    def move(self):
        if self.frozen: return
        Eold = self.E
        self.x += self.v
        self.v += G
        Unew = self.U
        Tnew = Eold - Unew
        if Tnew < 0:
            self.x[1] -= Tnew/G[1]
            Tnew = 0.0
            print 'Hey'
        if self.T > 0.0:
            self.v *= np.sqrt(0.95*Tnew/self.T)
        else:
            self.v *= 0

    def bounce(self):
        if self.x[0] > width - self.size:
            self.x[0] = 2*(width - self.size) - self.x[0]
            self.v[0] *= -BOUNCE_LOSS
            self.freeze()

        elif self.x[0] < self.size:
            self.x[0] = 2*self.size - self.x[0]
            self.v[0] *= -BOUNCE_LOSS
            self.freeze()

        if self.x[1] > height - self.size:
            self.x[1] = 2*(height - self.size) - self.x[1]
            self.v[1] *= -BOUNCE_LOSS
            self.freeze()

        elif self.x[1] < self.size:
            self.x[1] = 2*self.size - self.x[1]
            self.v[1] *= -BOUNCE_LOSS
            self.freeze()


def collide(p1,p2):
    dvec = p2.x - p1.x
    d = np.sqrt(dvec.dot(dvec))
    if d < p1.size + p2.size:
        dvec /= d
        p1.v += -2 * dvec.dot(p1.v) * dvec
        p1.v *= BOUNCE_LOSS
#        p2.v += -2 * dvec.dot(p2.v) * dvec
#        p1.v, p2.v = p2.v, p1.v

def create_fixed_rnd():
    fixed = []
    for n in range(20):
        size = 15 # random.randint(10, 20)
        x = random.randint(size, width-size)
        y = random.randint(size, height-size)
        new_p = Particle((x, y), size)
        new_p.v[0] = 0 # 5 * random.random()
        new_p.v[1] = 0 #5 * random.random()
        fixed.append(new_p)
    return fixed

def create_fixed_grid():
    fixed = []
    for i in range(17):
        for j in range(17):
            size = 3 # random.randint(10, 20)
            x1 = 0 + 50*i
            y1 = 0 + 50*j

            alpha = 0.2
            x = 100 + x1 * np.cos(alpha) - y1 * np.sin(alpha)
            y = x1 * np.sin(alpha) + y1 * np.cos(alpha)

            new_p = Particle((x, y), size)
            new_p.v[0] = 0 # 5 * random.random()
            new_p.v[1] = 0 # 5 * random.random()
            fixed.append(new_p)
    return fixed

fixed = create_fixed_grid()

moving = []
for n in range(1):
    c = (0,200,0) # tuple([random.randint(100,180) for i in range(3)])

    if True:
        p = Particle((390 + 3*n, 10), 5, colour=c)
        p.v[0] = 0.1 * random.random()
        p.v[1] = 0 #5 #+ 5 * random.random()

    moving.append(p)

while True:
    #screen.fill(background_colour)

    for f in fixed:
        f.display()

    screen.lock()
    E = 0.0

    for m in moving:
        m.colour = tuple([random.randint(100,180) for i in range(3)])
        m.move()
        m.bounce()
        for f in fixed: #[i+1:]:
            collide(m,f)
        m.display()
        E += m.E
    screen.unlock()

    print E #, moving[0].v[0], moving[1].v[0]
    pygame.display.flip()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit(0)
