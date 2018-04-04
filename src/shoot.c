#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <SDL/SDL_image.h>

#include "shoot.h"

#define BULLETSPEED 2
#define BULLETDAMAGES 10

static int id = 1;

Bullet  createBullet(Ship ship){
	Bullet  bullet;
	bullet = malloc( sizeof(Bullet) );
	if(bullet == NULL){
		printf("Erreur d'allocation mémoire\n");
		exit(0);
	}
	bullet->pos[X] = ship->pos[X];
	bullet->pos[Y] = ship->pos[Y];
	bullet->speed = BULLETSPEED;
	bullet->damages = BULLETDAMAGES;
	bullet->id = id;
	id++;

	return bullet;
}


void ajouterFinBList(BList *liste , Ship ship) {
	Bullet temp = createBullet( ship );

	if ( liste->taille == 0 ) {
		liste->first = temp;
		liste->last = temp;
	} else {
		liste->last->next = temp;
		temp->before = liste->last;
		liste->last = temp;
	}

	liste->taille++;
}
void afficherBList( BList *liste ) {
	Bullet actuel = liste->first;
  while ( actuel != NULL ) {
    //printf("%c", actuel->id);

    actuel = actuel->next;
  }
  //printf("\n");
}
int supprimerDernierBList( BList *liste ) {
  if ( liste->taille == 0 ) {
    printf("Liste vide\n");
    return 0;
  }
  return supprimerBList( liste, liste->last->id);
}
/*
	Supprime une unite de la liste doublement chainée
	L'unite a supprimer est identifiée par son id
*/
int supprimerBList( BList * liste, int id ) {
	Bullet tmp = liste->first;
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

void shoot(Ship ship){

}
