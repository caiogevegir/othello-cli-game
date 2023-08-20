#include "inc/board.h"

static const char _xCoord[8] = "ABCDEFGH";
static const char _yCoord[8] = "01234567";
static const char _emptyCell = '.';
static const char _space = ' ';
static const char _skip = '\n';

static
MovementStatus
_isMovementValid
(Board *b, Coordinates *c, char *strCoord)
{
  // Coordinates are outside board's range
  if ( 
    strCoord[0] < Y_MIN || strCoord[0] > Y_MAX || 
    strCoord[1] < X_MIN || strCoord[1] > X_MAX 
  )
  {
    printf("\n%s is outside board's range", strCoord);
    return OUTSIDE_RANGE;
  } 
  // Coordinates are inside range, but already occupied
  if ( b->table[c->y][c->x] != &_emptyCell )
  {
    printf ("\n%s is occupied", strCoord);
    return OCCUPIED;
  }
  // Movement is valid
  return VALID;
}


static void
_convertFromString
(Coordinates *c, char *strCoord)
{
  c->x = (strCoord[1] - X_MIN + X_OFFSET) * 2;
  c->y = (strCoord[0] - Y_MIN) + Y_OFFSET;
}

// -----------------------------------------------------------------------------

void
instantiateBoard
(Board *b)
{
  int i, j;

  b->table = (char***) malloc (10 * sizeof(char**));
  if ( b->table == NULL ) exit(1);

  for ( i=0; i<10; i++ )
  {
    b->table[i] = (char**) malloc (20 * sizeof(char*));
    if ( b->table[i] == NULL ) exit(1);   

    if ( i == 0 || i == 9 )
    {
      //  0 1 2 3 4 5 6 7 \n
      b->table[i][0] = b->table[i][1] = b->table[i][18] = (char*)&_space;
      for ( j=2; j<18; j+=2 )
      {
        b->table[i][j] = (char*)&_yCoord[(j/2)-1];
        b->table[i][j+1] = (char*)&_space;
      }
      b->table[i][19] = (char*)&_skip;
    }
    else
    {
      //N # # # # # # # N\n
      b->table[i][0] = b->table[i][18] = (char*)&_xCoord[i-1];
      b->table[i][1] = (char*)&_space;
      for ( j=2; j<18; j+=2 )
      {
        b->table[i][j] = (char*)&_emptyCell;
        b->table[i][j+1] = (char*)&_space;
      }
      b->table[i][19] = (char*)&_skip;
    }
  }

  b->numEmptyCells = NUM_BOARD_CELLS;
}


void
printBoard
(Board *b)
{
  int i, j;
  for ( i=0; i<10; i++ ) 
    for ( j=0; j<20; j++ ) 
      printf("%c", *(b->table[i][j]));
}


MovementStatus
placePieceOnBoard
(Board *b, Coordinates *c, char *piece, char *strCoord)
{
  MovementStatus mst;

  _convertFromString(c, strCoord);
  mst = _isMovementValid(b, c, strCoord);
  if ( mst != VALID ) return mst;

  b->table[c->y][c->x] = piece;
  b->numEmptyCells--;

  return mst;
}


int
capturePieces
(Board *b, Coordinates *c, char *atkPiece, char *defPiece)
{
  int i, j;
  int numCaptured = 0;

  // Check Horizontally Right →
  for ( i=1; b->table[c->y][c->x + i*2] == defPiece && (c->x + i*2)<20; i++ ) {}
  if ( (c->x + i*2)<20 && b->table[c->y][c->x + i*2] == atkPiece )
  {
    for ( j=0; j<i; j++, numCaptured++ ) b->table[c->y][c->x + 2*j] = atkPiece;
    numCaptured--;
  }
  // Check Horizontally Left ←
  for ( i=1; b->table[c->y][c->x - i*2] == defPiece && (c->x - i*2)>0; i++ ) {}
  if ( (c->x - i*2)>0 && b->table[c->y][c->x - i*2] == atkPiece )
  {
    for ( j=0; j<i; j++, numCaptured++ ) b->table[c->y][c->x - 2*j] = atkPiece;
    numCaptured--;
  }
  // Check Vertically Down ↓
  for ( i=1; b->table[c->y + i][c->x] == defPiece && (c->y + i)<10; i++ ) {}
  if ( (c->y + i)<10 && b->table[c->y + i][c->x] == atkPiece )
  {
    for ( j=0; j<i; j++, numCaptured++ ) b->table[c->y + j][c->x] = atkPiece;
    numCaptured--;
  }
  // Check Vertically Up ↑
  for ( i=1; b->table[c->y - i][c->x] == defPiece && (c->y - i)>0; i++ ) {}
  if ( (c->y - i)>0 && b->table[c->y - i][c->x] == atkPiece )
  {
    for ( j=0; j<i; j++, numCaptured++ ) b->table[c->y - j][c->x] = atkPiece;
    numCaptured--;
  }
  // Check Diagonally Down-Right ↓→
  for ( i=1; b->table[c->y + i][c->x + i*2] == defPiece && (c->x + i*2)<20 && (c->y + i)<10; i++ ) {}
  if ( (c->x + i*2)<20 && (c->y + i)<10 && b->table[c->y + i][c->x + i*2] == atkPiece )
  {
    for ( j=0; j<i; j++, numCaptured++ ) b->table[c->y + j][c->x + 2*j] = atkPiece;
    numCaptured--;
  }
  // Check Diagonally Up-Left ↑←
  for ( i=1; b->table[c->y - i][c->x - i*2] == defPiece && (c->x - i*2)>0 && (c->y - i)>0; i++ ) {}
  if ( (c->x - i*2)>0 && (c->y - i)>0 && b->table[c->y - i][c->x - i*2] == atkPiece )
  {
    for ( j=0; j<i; j++, numCaptured++ ) b->table[c->y - j][c->x - 2*j] = atkPiece;
    numCaptured--;
  }
  // Check Diagonally Down-Left ↓←
  for ( i=1; b->table[c->y + i][c->x - i*2] == defPiece && (c->x - i*2)>0 && (c->y + i)<10; i++ ) {}
  if ( (c->x - i*2)>0 && (c->y + i)<10 && b->table[c->y + i][c->x - i*2] == atkPiece )
  {
    for ( j=0; j<i; j++, numCaptured++ ) b->table[c->y + j][c->x - 2*j] = atkPiece;
    numCaptured--;
  }
  // Check Diagonally Up-Right ↑→
  for ( i=1; b->table[c->y - i][c->x + i*2] == defPiece && (c->x + i*2)<20 && (c->y - i)>0; i++ ) {}
  if ( (c->x + i*2)<20 && (c->y - i)>0 && b->table[c->y - i][c->x + i*2] == atkPiece )
  {
    for ( j=0; j<i; j++, numCaptured++ ) b->table[c->y - j][c->x + 2*j] = atkPiece;
    numCaptured--;
  }


  return numCaptured;
}
