#ifndef MENU_H
#define MENU_H

void drawBackground(GLuint textureID[]);
void drawSelectionBox(GLuint textureID[], float posButton[], int selectedDifficulty);
void drawButton(GLuint textureID[], float posButton[], int i, int id);

#endif
