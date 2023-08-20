#include <stdlib.h>
#include <string.h>

#define NAME_SIZE 16

typedef struct
{
  char name[NAME_SIZE];
  char piece;
  int numPieces;
  void (*input)(char*);
} Player;

void instantiatePlayer(Player *p, char *name, char piece, void input(char*));
void humanInput(char *strCoord);
void computerInput(char *strCoord);
