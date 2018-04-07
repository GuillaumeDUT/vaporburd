#include "libs.h"

#include "ennemies.h"

void ajouterFinEList(EList *liste, Ennemy ennemy) {

	if ( liste->taille == 0 ) {
		liste->first = ennemy;
		liste->last = ennemy;
	} else {
		liste->last->next = ennemy;
		ennemy->before = liste->last;
		liste->last = ennemy;
	}

	liste->taille++;
}
void afficherEList(EList *liste) {
	Ennemy actuel = liste->first;
  while ( actuel != NULL ) {
    actuel = actuel->next;
  }
}
int supprimerDernierEList(EList *liste) {
  if ( liste->taille == 0 ) {
    printf("Liste vide\n");
    return 0;
  }
  return supprimerEList( liste, liste->last->id);
}
/*
	Supprime une unite de la liste doublement chainée
	L'unite a supprimer est identifiée par son id
*/
int supprimerEList(EList * liste, int id) {
	Ennemy tmp = liste->first;
	int found = 0;
	while ( tmp != NULL && !found ) {
		if ( tmp->id == id ) {
			if ( liste->taille == 1 ) {
				liste->first = NULL;
				liste->last = NULL;
			} else if ( tmp->next == NULL ) {
				liste->last = tmp->before;
				liste->last->next = NULL;
			} else if ( tmp->before == NULL ) {
				liste->first = tmp->next;
				liste->first->before = NULL;
			} else {
				tmp->next->before = tmp->before;
				tmp->before->next = tmp->next;
			}
			found = 1;
			liste->taille--;
		} else {
			tmp = tmp->next;
		}
	}

	return 1;
}

void loopEList(Ship ship, EList *liste) {
  if(liste->taille == 0){
    return ;
  }
  Ennemy actuel = liste->first;
  Ennemy next;
  while ( actuel != NULL ) {
    next = actuel->next;
    /*
    moveEnnemy( actuel );
    */
    drawEnnemy( actuel );
    actuel = next;
  }
}
void drawEnnemy( Ennemy ennemy ) {
  printf("(%d) posX:%f posY:%f\n", ennemy->id, ennemy->pos[X], ennemy->pos[Y]);

  glPushMatrix();
  glColor3f(255, 0, 120);
  glTranslatef(
    ennemy->pos[X],
    ennemy->pos[Y],
    0);
  glScalef(
    ennemy->size,
    ennemy->size,
    1);
  drawCircle(1);
  glPopMatrix();
}