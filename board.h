#ifndef BOARD_H
#define BOARD_H

#include "Pieces.h"

class Square {
  private:
	Piece* peice;

  public:
	Square();
};

class Board {
  private:
	Square board[8][8];

  public:
	Square* operator[](int row);
};

#endif