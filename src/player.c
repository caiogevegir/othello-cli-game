#include "inc/player.h"

void
instantiatePlayer
(Player *p, char *name, char piece, void input(char*))
{
  strcpy(p->name, name);
  p->piece = piece;
  p->numPieces = 2;
  p->input = input;
}


void 
humanInput
(char *strCoord)
{
  gets(strCoord);
}


void
computerInput
(char *strCoord)
{
  // For testing purposes
  humanInput(strCoord);
}
