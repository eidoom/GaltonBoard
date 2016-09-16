import pygame
import sys
import random
import numpy as np

pygame.init()

width, height = 600, 400
screen = pygame.display.set_mode((width,height))
pygame.display.set_caption('Tutorial 1')

background_colour = (255,255,255)
screen.fill(background_colour)

G = 0#np.array([0,9.81e-2],dtype=np.float64)
DRAG = 1.0 #0.99
BOUNCE_LOSS = 1.0 #.75

class Particle(object):
    def __init__(self,(x,y),size):
        self.x = np.array([x,y],dtype=np.float64)
        self.size = size
        self.colour = (0,0,155)
        self.thickness = 1
        self.v = np.zeros(2,dtype=np.float64)

    def display(self):
        pygame.draw.circle(screen, 
                           self.colour,
                           (int(self.x[0]), int(self.x[1])),
                           self.size,
                           self.thickness
                           )

    def move(self):
        self.x += self.v
        #self.v += G
        #self.v *= DRAG

    def bounce(self):
        if self.x[0] > width - self.size:
            self.x[0] = 2*(width - self.size) - self.x[0]
            self.v[0] *= -BOUNCE_LOSS

        elif self.x[0] < self.size:
            self.x[0] = 2*self.size - self.x[0]
            self.v[0] *= -BOUNCE_LOSS

        if self.x[1] > height - self.size:
            self.x[1] = 2*(height - self.size) - self.x[1]
            self.v[1] *= -BOUNCE_LOSS

        elif self.x[1] < self.size:
            self.x[1] = 2*self.size - self.x[1]
            self.v[1] *= -BOUNCE_LOSS

def collide(p1,p2):
    dvec = p2.x - p1.x
    d = np.sqrt(dvec.dot(dvec))
    if d < p1.size + p2.size:
        dvec /= d
        p1.v += -2 * dvec.dot(p1.v) * dvec
#        p2.v += -2 * dvec.dot(p2.v) * dvec
#        p1.v, p2.v = p2.v, p1.v

ps = []
for n in range(15):
    size = 15 # random.randint(10, 20)
    x = random.randint(size, width-size)
    y = random.randint(size, height-size)
    new_p = Particle((x, y), size)
    new_p.v[0] = 0 # 5 * random.random()
    new_p.v[1] = 0#5 * random.random()
    ps.append(new_p)

p = Particle((200, 200), 10)
p.v[0] = 5 * random.random()
p.v[1] = 5 * random.random()

while True:
    screen.fill(background_colour)
    for p2 in ps:
        p2.display()

    screen.lock()
    E = 0.0
    if True:
#    for i,p in enumerate(ps):
        p.move()
        p.bounce()
        for p2 in ps: #[i+1:]:
            collide(p,p2)
        p.display()
        E += p.v.dot(p.v)
    screen.unlock()
    print E, ps[0].v[0],ps[1].v[0]
    pygame.display.flip()

    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit(0)
