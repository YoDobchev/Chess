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
	Square board[BOARD_SIZE][BOARD_SIZE];
	int checkExists;
	Position enPassantSquare, oldEnPassantSquare;

  public:
	// Used for clearing the en passant square after a move
	Board();
	Board(const String& serializedData);
	Square* operator[](const int row);
	String serialize() const;

	void setEnPassantSquare(const Position pos);
	Position getEnPassantSquare() const;
	void setPiece(Piece* piece, const Position pos);
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