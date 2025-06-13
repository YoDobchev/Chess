#ifndef BOARD_H
#define BOARD_H

// #include "Pieces.h"
#include "String.h"
#include "Types.h"
class Piece;

class Piece;

class Square {
  private:
	Piece* piece;
	bool isAttackedBy[2];

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
	Piece* getPieceAtPos(const Position pos) const;
	bool movePiece(const Position from, const Position to, const Player playerTurn, String& error);
};

#endif