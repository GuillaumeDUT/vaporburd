#include "libs.h"

#include "menu.h"



void drawMenuBackground(GLuint textureID[]) {

  /* Image de fond */
  glBindTexture(GL_TEXTURE_2D, textureID[9]);
  glPushMatrix();
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
  glPopMatrix();
  glBindTexture(GL_TEXTURE_2D, 0);
}
void drawGameBackground(GLuint textureID[], float globalTranslationTotal) {
  glBindTexture(GL_TEXTURE_2D, textureID[2]);
  glPushMatrix();
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
  glPopMatrix();
  glBindTexture(GL_TEXTURE_2D, 0);
}
void drawBossBackground(GLuint textureID[], float globalTranslationTotal) {
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
  float val = PI / 180.0;
  float offset = (SDL_GetTicks() / 5) % 360;
  float cosOffset = fmap(cos(offset * val), -1.0, 1.0, 1.02, 1.05);

  glBindTexture(GL_TEXTURE_2D, textureID[10]);
  glPushMatrix();
  {    
    glTranslatef(posButton[selectedDifficulty*2], posButton[selectedDifficulty*2+1], 0);
    glRotated(180,0,1,0);
    glScalef(1.02 * cosOffset, 1.1 * cosOffset,0);
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
}

void drawButton(GLuint textureID[], float posButton[], int i, int id) {
  //Dessine tous les boutons du menu 
  glBindTexture(GL_TEXTURE_2D, textureID[id]);
  glPushMatrix();
  {
    glTranslatef(posButton[i], posButton[i+1], 0);
    if(i>=10) 
      glScalef(2,2,0);
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
  }
  glPopMatrix();
}
void drawEndMenuButton(GLuint textureID[]) {
  glBindTexture(GL_TEXTURE_2D, textureID[17]);
  glPushMatrix();
  {
    glScalef(1.5,1.5,0);
    glTranslatef(0,-4,0);
    glColor3ub(255,255,255);
    glBegin(GL_QUADS);
    {
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
}

void drawTextWin(GLuint textureID[]) {
  glBindTexture(GL_TEXTURE_2D, textureID[18]);
  glPushMatrix();
  glScalef(4,4,0);
  glBegin(GL_QUADS);
  {
    glColor3ub(255,255,255);
    glTexCoord2f(0, 0);
    glVertex2f(-1.89 , +1);

    glTexCoord2f(1, 0);
    glVertex2f(+1.89, +1);

    glTexCoord2f(1, 1);
    glVertex2f(+1.89 , -1);

    glTexCoord2f(0, 1);
    glVertex2f(-1.89 , -1);
  }
  glEnd();
  glPopMatrix();
  glBindTexture(GL_TEXTURE_2D, 0);
}
void drawTextLose(GLuint textureID[]) {
  glBindTexture(GL_TEXTURE_2D, textureID[19]);
  glPushMatrix();
  glScalef(1.5,1.5,0);
  glBegin(GL_QUADS);
  {
    glColor3ub(255,255,255);
    glTexCoord2f(0, 0);
    glVertex2f(-3.79 , +1);

    glTexCoord2f(1, 0);
    glVertex2f(+3.79, +1);

    glTexCoord2f(1, 1);
    glVertex2f(+3.79 , -1);

    glTexCoord2f(0, 1);
    glVertex2f(-3.79 , -1);
  }
  glEnd();
  glPopMatrix();
  glBindTexture(GL_TEXTURE_2D, 0);
}