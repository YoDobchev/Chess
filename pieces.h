#ifndef PIECES_H
#define PIECES_H

#include "Types.h"
#include "String.h"

class Board;

class Piece {
  protected:
	Player color;
	Position pos_;

  public:
	Piece(Player color, Position pos);
	;
	Player getColor() const;
	virtual String getEmoji() const = 0;
	virtual bool checkIfValidMove(const Position to, const Board* board, String& error) const = 0;
};

class Pawn : public Piece {
  private:
	bool hasMoved;

  public:
	using Piece::Piece;
	String getEmoji() const override;

	bool checkIfValidMove(const Position to, const Board* board, String& error) const override;
};

class Rook : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;
	bool checkIfValidMove(const Position to, const Board* board, String& error) const override;
};

class Bishop : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;

	bool checkIfValidMove(const Position to, const Board* board, String& error) const override;
};

class Knight : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;

	bool checkIfValidMove(const Position to, const Board* board, String& error) const override;
};

class Queen : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;

	bool checkIfValidMove(const Position to, const Board* board, String& error) const override;
};

class King : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;

	bool checkIfValidMove(const Position to, const Board* board, String& error) const override;
};

#endif