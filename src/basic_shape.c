#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <SDL/SDL_image.h>

#define SEGMENTSCERCLES 64

#include "basic_shape.h"

void drawCircle(int full){
  GLenum type = full ? GL_POLYGON : GL_LINE_LOOP;
  glBegin(type);
    for(int i=0;i<SEGMENTSCERCLES;i++){
      glVertex2f(cos(i*(2*M_PI/SEGMENTSCERCLES))/2,sin(i*(2*M_PI/SEGMENTSCERCLES))/2);
    }
  glEnd();
}

void drawSquare(int full){
  GLenum type = full ? GL_QUADS : GL_LINE_LOOP;
  glBegin(type);
    glVertex2f(0.5,0.5);
    glVertex2f(0.5,-0.5);
    glVertex2f(-0.5,-0.5);
    glVertex2f(-0.5,0.5);
  glEnd();
}
