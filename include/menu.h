#ifndef MENU_H
#define MENU_H

#include "helpers.h"
#include "constants.h"

void drawMenuBackground(GLuint textureID[]);
void drawGameBackground(GLuint textureID[], float globalTranslationTotal);
void drawBossBackground(GLuint textureID[], float globalTranslationTotal);
void drawSelectionBox(GLuint textureID[], float posButton[], int selectedDifficulty);
void drawButton(GLuint textureID[], float posButton[], int i, int id);
void drawEndMenuButton(GLuint textureID[]);
void drawTextWin(GLuint textureID[]);
void drawTextLose(GLuint textureID[]);

#endif
