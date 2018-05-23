#ifndef MENU_H
#define MENU_H

void drawMenuBackground(GLuint textureID[]);
void drawGameBackground(GLuint textureID[], int globalTranslationTotal);
void drawBossBackground(GLuint textureID[], int globalTranslationTotal);
void drawSelectionBox(GLuint textureID[], float posButton[], int selectedDifficulty);
void drawButton(GLuint textureID[], float posButton[], int i, int id);

#endif
