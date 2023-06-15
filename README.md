# CGV-PROJECT
 Projectile motion


In the drawArrow function, glPushMatrix() is called at the beginning to save the current matrix state onto the stack.
This ensures that any transformations applied within the function will not affect other parts of the program that might be
drawing different objects.

After the arrow is drawn and all the transformations are applied, glPopMatrix() is called to restore the previous matrix state
from the stack. This ensures that any subsequent drawing commands in the program will not be affected


display ()
The direction taken by the projectile is determined by the resultant of the vertical and horizontal components of velocity.
the vertical component is affected by gravity and the duration of flight. It is given by vy= usin(theta)-gt where g denotes
acceleration due to gravity. The horizontal component of velocity is unaffected by gravity and is given by vx=ucos(theta).
According to paralleogram law, the angle made by the resulant is tan^-1 (vy/vx).
The atan2 function dtermines this is