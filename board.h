#ifndef BOARD_H
#define BOARD_H

#include "String.h"
#include "Types.h"

class Piece;

class Square {
  private:
	Piece* piece;
	bool isAttackedBy[2];
  int specialColor;

  public:
	Square();
  ~Square();
  int getSpecialColor() const;
  void setSpecialColor(const int color);
	void setPiece(Piece* piece);
	Piece* getPiece() const;
};

class Board {
  private:
	Square board[8][8];

  public:
  // Used for clearing the en passant square after a move
	Position enPassantSquare, oldEnPassantSquare;
	Board();
	Square* operator[](const int row);
	Piece* getPieceAtPos(const Position pos) const;
	bool movePiece(const Position from, const Position to, const Player playerTurn, String& error);
};

#endif