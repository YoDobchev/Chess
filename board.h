#ifndef BOARD_H
#define BOARD_H

#include "Pieces.h"
#include "Types.h"

class Piece;

class Square {
  private:
	Piece* piece;

  public:
	Square();
  // ~Square();
  void setPiece(Piece* piece);
  Piece* getPiece() const;
};

class Board {
  private:
	Square board[8][8];

  public:
	Board();
	Square* operator[](const int row);
	Piece* getPieceAtPos(Position pos) const;
};


#endif