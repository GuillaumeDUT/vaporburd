#ifndef OSU_READER_H
#define OSU_READER_H

typedef struct osunode {
  int x, y, time;
  struct osunode *next;
} *OSUNode;

typedef struct OSUList {
  OSUNode first;
  int taille;
} OSUList;

OSUList readOsuFile(char *filename);
OSUNode createOSUNode( int x, int y, int time );
void afficherOSUList( OSUList o );

#endif
