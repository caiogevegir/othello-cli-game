#include <stdio.h>

#include "inc/player.h"
#include "inc/board.h"

int
main 
(void)
{
  Player user, computer, *players[2];
  Board b;
  Coordinates c;
  char strCoord[2];
  int o, n;
  MovementStatus mst = WAITING_FOR_INPUT;

  instantiateBoard(&b);
  instantiatePlayer(&user, (char*)"Player", 'O', &humanInput);
  instantiatePlayer(&computer, (char*)"Computer", 'X', &computerInput);

  players[0] = &user;
  players[1] = &computer;

  // Setting Up
  placePieceOnBoard(&b, &c, &user.piece, (char*)"D3");
  placePieceOnBoard(&b, &c, &user.piece, (char*)"E4");
  placePieceOnBoard(&b, &c, &computer.piece, (char*)"D4");
  placePieceOnBoard(&b, &c, &computer.piece, (char*)"E3");

  // Game Loop
  for ( o=0; b.numEmptyCells>0; o=1-o, mst=WAITING_FOR_INPUT )
  {
    printf("  %c: %d\t%c: %d\n\n", user.piece, user.numPieces, computer.piece, computer.numPieces);
    printBoard(&b);
    printf("%s's turn: ", players[o]->name);

    while ( mst != VALID )
    {
      players[o]->input(strCoord);
      mst = placePieceOnBoard(&b, &c, &players[o]->piece, strCoord);
    }
    players[o]->numPieces++;

    n = capturePieces(&b, &c, &players[o]->piece, &players[1-o]->piece);
    players[o]->numPieces += n;
    players[1-o]->numPieces -= n;

    // Victory by domination
    if ( players[1-o]->numPieces == 0 && b.numEmptyCells < NUM_BOARD_CELLS-1 ) break;
  }

  // Results
  printf("  %c: %d\t%c: %d\n\n", user.piece, user.numPieces, computer.piece, computer.numPieces);
  printBoard(&b);
  if ( user.numPieces == 0 )
    printf("%s won by domination!", computer.name);
  else if ( computer.numPieces == 0 )
    printf("%s won by domination!", user.name);
  else if ( user.numPieces > computer.numPieces )
    printf("%s won by majority!", user.name);
  else if ( computer.numPieces > user.numPieces )
    printf("%s won by majority!", computer.name);
  else
    printf("The game is a draw!");

  return 0;
}
