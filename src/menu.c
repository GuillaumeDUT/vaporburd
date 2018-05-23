#include "libs.h"

#include "menu.h"

void drawBackground(GLuint textureID[], float posButton[], int selectedDifficulty) {

  /* Image de fond */
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureID[9]);
  glBegin(GL_QUADS);
  {
    glColor3ub(255,255,255);
    glTexCoord2f(0, 0);
    glVertex2f(-1.77 * 10, +1 *10);

    glTexCoord2f(1, 0);
    glVertex2f(+1.77 * 10, +1 *10);

    glTexCoord2f(1, 1);
    glVertex2f(+1.77 * 10, -1 *10);

    glTexCoord2f(0, 1);
    glVertex2f(-1.77 * 10, -1 *10);
  }
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureID[10]);
  glPushMatrix();{
    glTranslatef(posButton[selectedDifficulty*2],posButton[selectedDifficulty*2+1],0);
    glRotated(180,0,1,0);
    glScalef(1.04,1.1,0);
    glBegin(GL_QUADS);
    {
      //glColor4f(0.4,0,0.74,0.8);
      glColor3ub(255,255,255);
      glTexCoord2f(0, 0);
      glVertex2f(-3 , +1);

      glTexCoord2f(1, 0);
      glVertex2f(+3, +1);

      glTexCoord2f(1, 1);
      glVertex2f(+3 , -1);

      glTexCoord2f(0, 1);
      glVertex2f(-3 , -1);
    }
    glEnd();
  }
  glPopMatrix();
  glBindTexture(GL_TEXTURE_2D, 0);
  glDisable(GL_TEXTURE_2D);
}