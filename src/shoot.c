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

Bullet createBullet(Ship ship){
	Bullet bullet;
	bullet = (Bullet) malloc( sizeof(Bullet) );
	if(bullet == NULL){
		printf("Erreur d'allocation mémoire\n");
		exit(0);
	}
	//printf("ship pos X %f || ship pos Y %f");
	bullet->pos[X] = ship->pos[X];
	bullet->pos[Y] = ship->pos[Y];
	bullet->speed = BULLETSPEED;
	bullet->damages = BULLETDAMAGES;
	bullet->id = id;
	id++;
	bullet->next = NULL;
	bullet->before = NULL;

	return bullet;
}


void ajouterFinBList(BList *liste, Bullet bullet) {


	if ( liste->taille == 0 ) {

		liste->first = bullet;
		//printf("la liste prends en first la bullet créée\n");
		liste->last = bullet;

		//printf("la liste prends en last la bullet créée\n");
	} else {
		liste->last->next = bullet;
		bullet->before = liste->last;
		liste->last = bullet;
	}

	liste->taille++;
	printf(" taille liste : %d\n",liste->taille);
}
void afficherBList( BList *liste ) {
	if(liste->taille == 0){
		return ;
	}

	Bullet actuel = liste->first;
	//printf("%d posX %f posY %f\n",actuel->id,actuel->pos[X],actuel->pos[Y]);

  while ( actuel != NULL ) {

    //printf("%c", actuel->id);
		printf(" bullet || ");
    actuel = actuel->next;
  }

  printf("\n");
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

void shoot(Ship ship,BList *liste){
	Bullet bullet = createBullet( ship );
	ajouterFinBList(liste, bullet);
}
