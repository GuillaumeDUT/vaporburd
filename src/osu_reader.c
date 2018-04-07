#include "libs.h"

#include "osu_reader.h"

OSUList readOsuFile(char *filename) {
  printf("%s\n", filename);
  FILE *osuFile;
  osuFile = fopen(filename, "r");
  char line[256];
  char w1[256];
  int x, y, time, hitObjectFound = 0;
  OSUList list;
  list.first = NULL;
  list.taille = 0;
  OSUNode last;

  if ( osuFile == NULL ) {
    printf("Error opening the file\n");
    exit(0);
  }

  while( (fgets(line, 256, osuFile)) != NULL ) {
    sscanf(line, "%s", w1);

    /* La ligne est un evenement osu */
    if ( hitObjectFound == 1 ) {
      sscanf(line, "%d,%d,%d", &x, &y, &time);

      if ( list.first == NULL ) {
        last = createOSUNode(x, y, time);
        list.first = last;
      } else {
        last->next = createOSUNode(x, y, time);
        last = last->next;
      }
      list.taille++;
    }
    if ( strcmp(w1, "[HitObjects]") == 0 ) {
      hitObjectFound = 1;
    }
  }
  
  // afficherOSUList( list );
  
  fclose(osuFile);
  return list;
}
OSUNode createOSUNode( int x, int y, int time ) {
  OSUNode tmp = (OSUNode) malloc( sizeof(*tmp) );
  if ( tmp == NULL ) {
    printf("Erreur d'allocation mémoire\n");
    exit(0);
  }
  tmp->x = x;
  tmp->y = y;
  tmp->time = time;
  tmp->next = NULL;
  return tmp;
}
void afficherOSUList( OSUList o ) {
  if ( o.taille == 0 ) return;
  OSUNode actuel = o.first;
  while ( actuel != NULL ) {
    printf("%d,%d,%d -> ", actuel->x, actuel->y, actuel->time);
    actuel = actuel->next;
  }
  printf("NULL\n");
  printf("Taille: %d", o.taille);
}