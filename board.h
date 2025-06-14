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
	bool getAttackedBy(Player player) const;
	void setAttackedBy(Player player);
	void clearAttackedBy();
	Piece* getPiece() const;
};

class Board {
  private:
	Square board[8][8];
	int checkExists;

  public:
	// Used for clearing the en passant square after a move
	Position enPassantSquare, oldEnPassantSquare;
	Board();
	Board(const String& serializedData);
	Square* operator[](const int row);
	String serialize() const;

	Piece* getPieceAtPos(const Position pos) const;
	void calculateSquares();
	void clearAttackedSquares();
	void setCheckExists(int exists);
	int getCheckExists() const;
	bool getCheckmate() const;
	bool getStalemate() const;
	bool movePiece(const Position from, const Position to, const Player playerTurn, String& error);
};

#endif