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
	// -1 - no check, 0 - white in check, 1 - black in check
	int checkExists;
	// Used for clearing the en passant square after a move
	Position enPassantSquare, oldEnPassantSquare;

  public:
	Board();
	Board(const String& serializedData);
	~Board() = default;
	Square* operator[](const int row);
	String serialize() const;

	void setEnPassantSquare(const Position pos);
	Position getEnPassantSquare() const;
	Position getOldEnPassantSquare() const;
	void setPiece(Piece* piece, const Position pos);
	Piece* getPieceAtPos(const Position pos) const;
	void calculateSquares();
	void clearAttackedSquares();
	void setCheckExists(int exists);
	int getCheckExists() const;
	bool movePiece(const Position from, const Position to, const Player playerTurn, String& error);
};

#endif