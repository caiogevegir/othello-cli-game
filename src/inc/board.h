#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NUM_BOARD_CELLS 64

#define Y_MIN 0x41
#define Y_MAX 0x48
#define Y_OFFSET 1

#define X_MIN 0x30
#define X_MAX 0x37
#define X_OFFSET 1

typedef enum
{
  VALID,
  OUTSIDE_RANGE,
  OCCUPIED,
  WAITING_FOR_INPUT
} MovementStatus;

typedef struct {
  int x;
  int y;
} Coordinates;

typedef struct {
  char ***table;
  int numEmptyCells;
} Board;

void instantiateBoard(Board *b);
void printBoard(Board *b);
MovementStatus placePieceOnBoard(Board *b, Coordinates *c, char *piece, char *strCoord);
int capturePieces (Board *b, Coordinates *c, char *atkPiece, char *defPiece);
