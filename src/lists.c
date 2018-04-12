#include "libs.h"

#include "lists.h"

void ajouterFinList(List *liste , Entity e){
  if ( liste->taille == 0 ) {
    liste->first = e;
    liste->last = e;
  } else {
    liste->last->next = e;
    e->before = liste->last;
    liste->last = e;
  }
  liste->taille++;  
}
void afficherList(List *liste){
  if(liste->taille == 0){
    return;
  }
  Entity actuel = liste->first;
  //printf("%d posX %f posY %f\n",actuel->id,actuel->pos[X],actuel->pos[Y]);
  while ( actuel != NULL ) {
    printf("%d->", actuel->id);
    actuel = actuel->next;
  }
  printf("\n");
}
int supprimerDernierList(List *liste){
  if ( liste->taille == 0 ) {
    printf("Liste vide\n");
    return 0;
  }
  return supprimerList( liste, liste->last->id);  
}
/*
	Supprime une entity de la liste doublement chainée
	L'entity a supprimer est identifiée par son id
*/
int supprimerList(List *liste, int id){
  Entity tmp = liste->first;
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
int deleteList(List *liste) {
  while ( liste->taille != 0 ) {
    supprimerDernierList(liste);
  }
  return 1;
}