Notes:
PLAYER, P_PROJECTILE, A_PROJECTILE, ALIEN, PADDLE, BLOCK, SPECIAL_BLOCK, PBALL, NON_BOTTOM_WALL and 
BOTTOM_WALL were removed from enum in body.h. Only types relevant to the Rolly Poly game were left in 
order to clean up code. To ensure proper behavior of old demos, these types should be added back to the
list. The deletion of types specific to old demos is noted here based on advice from Prof. Blank.

make_pac(), make_alien(), and make_oval() were removed from polygon.c. Only shape generators relevant to 
the Rolly Poly game were left in order to clean up code. To ensure proper behavior of old demos, these 
functions should be added back to polygon.c. The deletion of functions specific to old demos is noted 
here based on advice from Prof. Blank.# Rolly-Polly-Private
