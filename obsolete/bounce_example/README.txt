A simple bouncing balls demo to demonstrate Pygame sprites. It makes a
'fish tank' and fills it with random blocks, then drops a large number of
random balls in it that bounce around off the walls, blocks, and each
other.  After they settle down enough they disappear and respawn. Balls
are not perfectly elastic (they lose energy on bounces) and gravity and ball
mass are taken into account for collisions.

I wrote this back in 2005 as my first Pygame program, and looking back,
this is pretty naive, but I can still just watch it for quite a while.

You can just go in and hack the settings in the config file at top.

Limitations are:

  - No sound.
  - Collisions are rectangular instead of circular.
  - Physics aren't real, just 'real enough', which means:
  - Collisions are treated as separate sets of two-body collisions, so
    overlap will occur when for example ball A and B collide, so bounce
    back, and C and D collide, so bounce back, but now C and B are 
    overlapping each other. Then they're sort of stuck!

Ron
