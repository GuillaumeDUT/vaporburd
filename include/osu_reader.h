#ifndef OSU_READER_H
#define OSU_READER_H

typedef struct node {
  int x, y, time;
  struct node *next;
} *OSUNode;

typedef struct OSUList {
  OSUNode first;
  int taille;
} OSUList;

OSUList readOsuFile(char *filename);
OSUNode createOSUNode( int x, int y, int time );
void afficherOSUList( OSUList o );

#endif
