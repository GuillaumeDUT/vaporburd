#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

#include "ship.h"
#include "basic_shape.h"


void drawShip(){

	glPushMatrix();
		glTranslatef(-1,0,0);
		drawCircle(1);
	glPopMatrix();

}
