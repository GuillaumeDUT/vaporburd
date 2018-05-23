#include "libs.h"

#include "menu.h"

void drawMenuBackground(GLuint textureID[]) {

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

}
void drawGameBackground(GLuint textureID[], int globalTranslationTotal) {
  glBindTexture(GL_TEXTURE_2D, textureID[2]);
  glTranslatef(42+globalTranslationTotal*0.58,0,0);
  glBegin(GL_QUADS);
  {
    glColor3ub(255,255,255);
    glTexCoord2f(0, 0);
    glVertex2f(-5.33 *10, +1 *10);

    glTexCoord2f(1, 0);
    glVertex2f(+5.33 *10, +1 *10);

    glTexCoord2f(1, 1);
    glVertex2f(+5.33 *10, -1 *10);

    glTexCoord2f(0, 1);
    glVertex2f(-5.33 *10, -1 *10);
  }
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
}
void drawBossBackground(GLuint textureID[], int globalTranslationTotal) {
  glBindTexture(GL_TEXTURE_2D, textureID[16]);
  glTranslatef(globalTranslationTotal,0,0);
  glRotatef(20-(globalTranslationTotal*5),0,0,1);
  glBegin(GL_QUADS);
  {
    glColor3ub(255,255,255);
    glTexCoord2f(0, 0);
    glVertex2f(-1 *14, +1 *14);

    glTexCoord2f(1, 0);
    glVertex2f(+1 *14, +1 *14);

    glTexCoord2f(1, 1);
    glVertex2f(+1 *14, -1 *14);

    glTexCoord2f(0, 1);
    glVertex2f(-1 *14, -1 *14);
  }
  glEnd();
  glBindTexture(GL_TEXTURE_2D, 0);
}

void drawSelectionBox(GLuint textureID[], float posButton[], int selectedDifficulty) {
  /* Cadre de selection */
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, textureID[10]);
  glPushMatrix();{
    glTranslatef(posButton[selectedDifficulty*2],posButton[selectedDifficulty*2+1],0);
    glRotated(180,0,1,0);
    glScalef(1.04,1.1,0);
    glBegin(GL_QUADS);
    {
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

void drawButton(GLuint textureID[], float posButton[], int i, int id) {
  glPushMatrix();
  {
    //printf("button %d",i-1+10);
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, textureID[id]);
    glTranslatef(posButton[i],posButton[i+1],0);
    if(i>=10){
      glScalef(2,2,0);
    }
    glBegin(GL_QUADS);
    {
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
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
  }
  glPopMatrix();
}