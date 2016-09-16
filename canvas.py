import pygame
import sys
import random

pygame.init()

width, height = 600, 400
screen = pygame.display.set_mode((width,height))
pygame.display.set_caption('Tutorial 1')

background_colour = (255,255,255)
screen.fill(background_colour)


class Particle(object):
    def __init__(self,(x,y),size):
        self.x = x
        self.y = y
        self.size = size
        self.colour = (0,0,155)
        self.thickness = 1

    def display(self):
        pygame.draw.circle(screen, 
                           self.colour,
                           (self.x, self.y),
                           self.size,
                           self.thickness
                           )

p1 = Particle((150,50), 15)
p1.display()




pygame.display.flip()
while True:
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            pygame.quit()
            sys.exit(0)
