#!/usr/bin/env python
"""
Copyright 2005 Ron Dippold - Creative Commons

This is written for Pygame v1.6

A simple bouncing balls demo to demonstrate Pygame sprites. It makes a
'fish tank' and fills it with random blocks, then drops a large number of
random balls in it that bounce around off the walls, blocks, and each
other.  After they settle down enough they disappear and respawn. There
is friction (the balls are semi-squishy) and gravity.

This is my first Pygame program, intended to figure out how it
works. Pygame actually handles quite an impressive number of sprites
colliding with each other - I can easily drop 100 balls, which are all
hitting each other and the walls and other blocks and everything runs
quite smoothly (on a 3ghz pentium, admittedly, but that's a lot of
work for O(N^2)).

The physics used here are not 'real', just 'real enough'! The sprite
interaction was more important here than the physics. I simplistically treat
multibody collisions as separate sets of two-body collisions, and
  ** therefore you'll see some balls overlapping each other **
we don't forbid it, just discourage it.

Furthermore, the balls aren't really colliding, the sprite *rectangles* are
colliding, so you may see some false hits when a small 'ball' hits the
empty corner of a large 'ball'.

One neat thing is that we don't have to know how large the ball is at all
for the physics! We just have to know that it collided with something else,
and Pygame takes care of that. We take advantage of this with BALLFILE -
8x8, 12x12, or 16x16, the size doesn't really matter.

There is a configuration section at the top of the program where you can
set some initial variables. I didn't bother using command line options.

V0.01 Mar 9 '05 - Write initial version before heading to FLA for a week
V0.90 Mar22 '05 - Document a bit more, add ball color support
V1.00 Mar23 '05 - Add multiple simultaneous ball types
V1.01 Mar23 '05 - COLLIDE_FUZZY for much better behavior
V1.02 Mar23 '05 - Make balls sink out of sight instead of just going away
V1.03 Mar24 '05 - Do current_time-last_time calc only once per loop.
                  Move walls into background, don't bother drawing every frame.
"""


# -----------------------------------------------
# Configuration section
#
# Change any of these you want, I'm not really
# checking for if you do something silly like set
# a window size of 2 pixels.
# -----------------------------------------------

# size of window
WINDOWX = 800
WINDOWY = 400

# Total ball count
BALL_COUNT = 50

# Ball pictures. These must be gifs, and the first color in the palette must be the
# ball color - we use that to do quick recoloring of the ball.
BALLFILES = ( "ball16x16.gif", "ball12x12.gif", "ball8x8.gif",
              "smile12x12.gif" )

# wall width
WALL_WIDTH = 2

# Extra boxes to add. We divide the window up into BOX_NX*BOX_NY boxes, then
# generate BOXES boxes randomly.
BOX_NX = 40
BOX_NY = 20
BOXES = (BOX_NX*BOX_NY)/20

# pixels per second per second
GRAVITY = 20

# Collision 'friction' factor. We use ELASTIC for everything except
# ELASTIC_FLOOR for a Ball bouncing off the horizontal part of a Wall.
# I told you this wasn't real physics!
ELASTIC = 0.95
ELASTIC_FLOOR = 0.80


# When two balls collide with each other, don't process any more collisions
# between them for this many frames to give them time to get apart.
#
# This is sort of a big hack that splits the difference between allowing balls
# to repeatedly collide when they're inside each other (which makes them act
# like wobbly blobs of jello) and not allowing them to collide again till
# they're not touching (which can let them pass entirely through each other).
#
# This happens because we're not simulating exact time of contact and because
# process multiple simultaneous collisions as pairs. It's not real physics!
#
# There's no right value here, different values give different amusing results.
# You might try it with 1.
COLLIDE_FUZZY = 25


# -----------------------------------------------
# Imports
# -----------------------------------------------

import random
import pygame
import math
from pygame.locals import *

# -----------------------------------------------------------------------------
#
# class Wall
#
# -----------------------------------------------------------------------------

class Wall(pygame.sprite.Sprite):
    """
    A Wall is just a nonmoving rectangle (but still a sprite) that acts as an
    obstacle. It can be an obstacle block right in the middle of the tank or
    one of the restraining walls on the left, right, and bottom.

    All walls are orthoganal since Pygame sprite collision only handles
    rectangles - something at a 45 degree angle would match a collision for
    the entire area from _xy1 to _xy2 even though 90% of it was empty. I
    could do arbitrary plane collisions, but that's not the point of this.
    """

    def __init__( self, _xy1, _xy2, _color=[255,0,0] ):
        """
        _xy1 is the upper left corner [x,y]
        _xy2 is the lower right corner [x,y]
        _color is an optional rgb triplet [r,g,b]
        """

        # must call Sprite.__init__ !
        pygame.sprite.Sprite.__init__(self)
      
        # Create the image that will be displayed. The math is just figuring
        # out the width and height of the needed rectangle.
        self.image = pygame.Surface( [_xy2[0]-_xy1[0]+1, _xy2[1]-_xy1[1]+1 ] )
        self.image.fill(_color)

        # Top left is just first coordinate
        self.rect = self.image.get_rect()
        self.rect.topleft = _xy1;

        # just in case
        self.id = 0

    def do_hit( self, ball ):
        """
        The logic for a ball colliding with us - the ball bounces, we don't!

        Notice that we actually do change the trajectory of the ball that
        collided with us, which may seem a little strange.
        """

        # Get our starting speeds
        speedx = ball.speed[0] * ELASTIC
        speedy = ball.speed[1] * ELASTIC_FLOOR

        # Bounce cleanly off a 'floor' - no change in horizontal speed
        if self.rect.left < ball.rect.centerx < self.rect.right:
            vbounce = ball.rect.centery - self.rect.centery
            hbounce = 0

        # Bounce cleanly off a 'wall' - no change in vertical speed
        elif self.rect.top < ball.rect.centery < self.rect.bottom:
            vbounce = 0
            hbounce = ball.rect.centerx - self.rect.centerx
            
        # Otherwise, bounce off the corner
        else:
            vbounce = ball.rect.centery - self.rect.centery
            hbounce = ball.rect.centerx - self.rect.centerx

        # Do the actual speed modifications depending on which side
        # we hit on.
        if vbounce>0:
            speedy = abs(speedy)
        elif vbounce<0:
            speedy = -abs(speedy)
        if hbounce>0:
            speedx = abs(speedx)
        elif hbounce<0:
            speedx = -abs(speedx)

        # And finally assign it
        ball.speed = [ speedx, speedy ]
        

# -----------------------------------------------------------------------------
#
# class Ball
#
# -----------------------------------------------------------------------------

class Ball(pygame.sprite.Sprite):
    """
    A Ball is well, a ball. It has a size (though we don't care about that,
    only Pygame does), mass, and color.
    """


    # ----------------------------
    # Class 'static' variables
    # ----------------------------
    
    # Preload all our bitmap files
    types = {}
    for f in BALLFILES:
        image = pygame.image.load(f)
        # since the 'ball' technically doesn't need to be square, get the avg radius.
        # Do not use a^2+b^2=c^2 because the full diagonal is far too long.
        radius = (image.get_width()+image.get_height())/2.0
        types[f] = {
            # file name just for debugging
            "file" :    f,
            # the loaded image - do not convert yet, we need palette!
            "image" :   image,
            # Mass is actually 4/3 * pi * r^3, but we just care about ratio
            "mass"  :   pow(radius,3),
            # Cached colorized versions - none yet
            "colors" :  {}
            }

    # Don't spit out all sorts of stuff about collisions
    verbose_collision = False

    # Keep track of ball IDs for debugging
    next_id = 1

    # How many active balls
    active_count = 0

    # --------------------------
    # Construct a new Ball
    # --------------------------
    def __init__( self, _type, _color, _pos, _speed ):
        """
        type is the ball type name from BALLFILES
        color is (r,g,b) - a (tuple), not a [list]
        startpos is starting [x,y]
        startspeed is starting [xspeed,yspeed] in pixels/second
        """

        # init the sprite, set up the image
        pygame.sprite.Sprite.__init__( self )

        # Cache the color converted version of each color. This is only useful
        # since we've limited the number of colors to 7 - if r,g,b could each
        # vary fully from 0-255 we'd be wasting our time doing any caching.
        type = Ball.types[_type]
        if not type["colors"].get(_color):
            image = type["image"]
#            image.set_palette_at( 0, _color )

            # Now set the image color key, and RLE accelerate it
            image.set_colorkey( image.get_at((0,0)), RLEACCEL )
            # Save it in the cache map
            type["colors"][_color] = image.convert()

        # Our ball image is just the image in the color cache
        self.image = type["colors"][_color]

        # our own id - just for debugging purposes
        self.id = Ball.next_id
        Ball.next_id += 1

        # One more ball is active
        Ball.active_count += 1

        # Mass for collisions
        self.mass = type["mass"]

        # Get our rectangle and set position
        self.rect = self.image.get_rect()
        self.rect.topleft = _pos

        # We need to keep a floating point representation of pos, integer pixel
        # position just isn't accurate enough to track fractional pixel speeds
        self.pos = [ _pos[0], _pos[1] ]
        self.speed = _speed

        # Start us off with an empty list of things not to collide with - see COLLIDE_FUZZY
        self.dont_collide = {}


    # override the kill so we can decrement the number of active balls
    def kill( self ):
        #print "Killing ball", self.id
        Ball.active_count -= 1
        pygame.sprite.Sprite.kill( self )
        

    #
    # Move a Ball depending on how much time has passed
    #
    def update( self, seconds ):
        """
        Given how many (decimal) secons have passed, figure out how far to move
        the ball based on how many (milli)seconds have passed since the last
        time we were called. Also add GRAVITY to the y velocity.
        """

        # then move self.speed*factor pixels
        self.pos[0] += self.speed[0] * seconds
        self.pos[1] += self.speed[1] * seconds

        # Change the sprite location
        self.rect.topleft = self.pos

        # Add gravity acceleration to the velocity
        self.speed[1] += GRAVITY * seconds

    #
    # check to see if the ball is out or has 'stopped' on the bottom
    #
    def out( self ):
        """
        Return True if the ball is out of the tank.
        """
        return self.pos[1]>WINDOWY or self.pos[0]<0 or self.pos[0]>WINDOWX

    def stopped( self ):
        """
        Return True if the ball is stopped (or close enough) on the bottom.
        """
        return self.pos[1]>WINDOWY or \
                   ( self.pos[1]>=(WINDOWY-WALL_WIDTH-self.rect.height-1 ) and 
                     abs(self.speed[0])<0.5 and abs(self.speed[1])<0.5 )

    #
    # Hit another ball
    #
    def do_hit( self, ball ):
        """
        The logic for another ball hitting us. We only modify the speed for
        BOTH balls (might as well only do the calculation once), then skip
        us hitting them separately.
        """

        # This is much easier than it looks. Momentum is conserved, so the difference in
        # speed is basically just ball.speed-self.speed, but we have to figure out what
        # the angle of impact is and apply the speed difference at that angle.
        #    See http://www.plasmaphysics.org.uk/collision2d.htm for detailed info

        # Difference in ball speeds
        deltavx = ball.speed[0]- self.speed[0]
        deltavy = ball.speed[1]- self.speed[1]

        # velocity difference angle - note this is ball minus self
        gammav = math.atan2( deltavy, deltavx )

        # position difference angle - note this is self minus ball
        gammaxy = math.atan2( self.rect.centery-ball.rect.centery,
                              self.rect.centerx-ball.rect.centerx )

        # relative angle of impact
        #alpha = math.asin(math.sin(gammaxy-gammav))
        alpha = gammaxy-gammav

        # Figure how much is x and how much is y
        # a = math.tan( gammav + alpha )
        a = math.tan( gammaxy )

        # Mass ratio between the balls
        mratio= self.mass/ball.mass

        # Now figure the difference in x speed - then compensate for mass ratio
        deltavx2 = (deltavx + a*deltavy) / (1+a*a)
        deltavx2 *= 2/(1+mratio)

        # difference in y speed is just difference in x * a
        deltavy2 = deltavx2 * a

        if Ball.verbose_collision:
            print "Self %2d -  pos:(%.2f,%.2f) speed:(%.2f,%.2f) mass:%.2f" %  \
                  ( self.id, self.pos[0], self.pos[1], self.speed[0], self.speed[1], self.mass )
            print "     %2d -  pos:(%.2f,%.2f) speed:(%.2f,%.2f) mass:%.2f" % \
                  ( ball.id, ball.pos[0], ball.pos[1], ball.speed[0], ball.speed[1], ball.mass )
            print "           deltav:(%.2f,%.2f) gammav:%.2f gammaxy:%.2f alpha:%.2f a:%.2f" % \
                  ( deltavx, deltavy, gammav, gammaxy, alpha, a )
            print "           deltav2:(%.2f,%.2f) mratio:%.2f" % ( deltavx2, deltavy2, mratio )

        # and apply the velocity differences
        self.speed[0] = self.speed[0]*ELASTIC2 + deltavx2
        self.speed[1] = self.speed[1]*ELASTIC2 + deltavy2

        ball.speed[0] = ball.speed[0]*ELASTIC2 - deltavx2*mratio
        ball.speed[1] = ball.speed[1]*ELASTIC2 - deltavy2*mratio

        if Ball.verbose_collision:
            print "     %2d*-  speed:(%.2f,%.2f)" % ( self.id, self.speed[0], self.speed[1] )
            print "     %2d*-  speed:(%.2f,%.2f)" % ( ball.id, ball.speed[0], ball.speed[1] )


        # Don't collide with us again for a while - see COLLIDE_FUZZY docs
        ball.dont_collide[self] = COLLIDE_FUZZY

        # Ball just collided with us, don't collide with ball (would double effect!)
        self.dont_collide[ball] = COLLIDE_FUZZY


# -----------------------------------------------------------------------------
#
# Helper functions
#
# -----------------------------------------------------------------------------

#
# generate a new ball at a random location
#
def newball():
    """
    Generate a new ball at a semi-random location at the top of the tank.
    Speed is also semi-random, color is random

    Note - this can return a ball falls out of the window! No big deal, we
            just kill it when it falls below the bottom (off the screen)

    """

    return Ball(
        _type =     random.choice(BALLFILES),
        _color =    ball_color(),
        _pos =      [ (WINDOWX-20)*random.random()+10, -(random.random()*60+8) ],
        _speed =    [ random.random()*80-40, random.random()*20 ]
        )


#
# Run collision of one group with another
#
def collide( balls, obstacles ):
    """
    Run results of collision of one group of sprites with another.
    """

    # Let the engine find the collisions
    hits = pygame.sprite.groupcollide( balls, obstacles, False, False )

    # Now loop through the collision sets which is a dictionary of ball/list of hit items
    for ball, hititems in hits.iteritems():

        # We know 'ball' collided with 'hititems' - so for every hititem...
        for hititem in hititems:
            
            # Don't collide with ourself
            if hititem is ball: continue

            # don't collide with x if x just collided with us - see COLLIDE_FUZZY
            if hititem in ball.dont_collide:
                continue

            # Calculate the hit effect
            hititem.do_hit( ball )


BRIGHT_COLORS = ( (0,0,255), (0,255,0), (0,255,255), \
                  (255,0,0), (255,0,255), (255,255,0), (255,255,255) )
def random_brightcolor():
    """
    Return a (r,g,b) tuple with r/g/b either 0 or 255 (but not all 0 at once)
    """
    return random.choice(BRIGHT_COLORS)

def block_color():
    """
    Used to return a random_brightcolor(), but that looked like
    clown vomit so now we just return red.
    """
    return (255, 0, 0)

def ball_color():
    """
    return a random_brightcolor()
    """
    return random_brightcolor()


# -----------------------------------------------------------------------------
#
# Main loop
#
# -----------------------------------------------------------------------------
if __name__ == "__main__":


    # Enforce a minimum of 1
    COLLIDE_FUZZY = max(COLLIDE_FUZZY,1)

    # Used a lot in collisions so calculate it up front
    ELASTIC2 = (ELASTIC+1)/2

    # Create the window
    pygame.init()
    pygame.display.set_caption( `BALL_COUNT` + " Bouncy Balls")
    screen = pygame.display.set_mode( (WINDOWX,WINDOWY) )

    # Create background, make it all black for now
    background = pygame.Surface( screen.get_size() ).convert()
    background.fill( (0,0,0) )

    # ----------------------------
    # Create walls, then draw them immediately on background
    # ----------------------------

    # walls don't need to clear, just render, but if we don't use RenderUpdates this
    #   will update the entire window since it covers the entire thing. So we
    #   make it a RenderUpdates type sprite.
    #walls = pygame.sprite.RenderUpdates()
    walls = pygame.sprite.RenderPlain()
    
    # left wall
    walls.add( Wall( [0,0], [WALL_WIDTH,WINDOWY-1] ) )
    # bottom wall
    walls.add( Wall( [0,WINDOWY-1-WALL_WIDTH], [WINDOWX-1,WINDOWY-1] ) )
    # right wall
    walls.add( Wall( [WINDOWX-1-WALL_WIDTH,0], [WINDOWX-1,WINDOWY-1]    ) )

    # Add some extra box 'walls' randomly
    xsize = WINDOWX/BOX_NX
    ysize = WINDOWY/BOX_NY

    for i in range(BOXES):
        x = int( random.random()*BOX_NX ) * xsize
        y = int( random.random()*BOX_NY ) * ysize
        walls.add( Wall (
            _xy1 =  [ x, y ],
            _xy2 =  [ x+xsize-1, y+ysize-1 ],
            _color= block_color()
            ))

    # Now move the walls to the background.
    # This wouldn't work if we had any moving walls.
    walls.draw( background )

    # ----------------------------
    # Make bouncing ball group
    # ----------------------------

    # Our 'live' balls
    balls = pygame.sprite.RenderUpdates()

    # dead balls that are sinking out of sight
    deadballs = pygame.sprite.RenderUpdates()

    # Get ready to start adding balls right away
    next_newball_time = 0

    # ----------------------------
    # Our infinite 'game' loop
    # ----------------------------

    # Start out with putting background on the screen
    screen.blit( background, (0,0) )
    pygame.display.update()
    current_time = pygame.time.get_ticks()
    
    while pygame.event.poll().type != KEYDOWN:

        # Figure out how many seconds it's been
        last_time = current_time
        current_time = pygame.time.get_ticks()
        seconds = (current_time-last_time)/1000.0
        
        # Add a new ball at most every half second
        if Ball.active_count<BALL_COUNT and current_time>=next_newball_time:
            balls.add( newball() )
            next_newball_time = current_time + 500

        # ---------------------
        # Move balls
        # ---------------------

        balls.update( seconds )
        deadballs.update( seconds )

        # ---------------------
        # Ball collision
        # ---------------------

        # Decrement our don't collide counts - see COLLIDE_FUZZY for docs
        for ball in balls.sprites():
            newdict = {}
            for item,count in ball.dont_collide.iteritems():
                if count>1:
                    newdict[item] = count-1
            ball.dont_collide = newdict
            
        # collide balls with balls, then balls with walls.
        # This makes walls are more 'solid'.
        # Notice deadballs don't hit anything!
        collide( balls, balls )
        collide( balls, walls )

        # If any deadballs have sunk out of sight, remove them
        for ball in deadballs.sprites():
            if ball.out():
                ball.kill()
                
        # See if any balls have ground to a stop. If so make it disappear.
        for ball in balls.sprites():
            # If ball is out of the tank, just kill it outright
            if ball.out():
                ball.kill()

            # if the ball is stopped, sink it out of sight
            if ball.stopped():
                balls.remove(ball)
                deadballs.add(ball)

        # ---------------------
        # draw the balls and walls
        # ---------------------
        
        # Start with empty list of draw changes
        changelist = []

        # move balls and draw them
        changelist += balls.draw(screen)
        changelist += deadballs.draw(screen)

        # We don't need to redraw the walls since we drew them on background
        # changelist += walls.draw(screen)

        # Actually update the screen
        pygame.display.update(changelist)

        # clear sprites for next time
        balls.clear( screen, background )
        deadballs.clear( screen, background )

        # wait
        pygame.time.delay(10)
        

        

    
