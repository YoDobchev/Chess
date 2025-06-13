#ifndef PIECES_H
#define PIECES_H

// #include "Board.h"
#include "Types.h"
#include "String.h"

class Piece {
  protected:
	// Square* pos;
	Player color;

  public:
	Piece(Player color);
	virtual String getEmoji() = 0;
	// virtual bool checkIfValidMove() = 0;
};

class Pawn : public Piece {
  private:
  public:
	using Piece::Piece;

	String getEmoji() override;
	// bool checkIfValidMove() override;
};

class Rook : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() override;
	// bool checkIfValidMove() override;
};

class Bishop : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() override;

	// bool checkIfValidMove() override;
};

class Knight : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() override;

	// bool checkIfValidMove() override;
};

class Queen : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() override;

	// bool checkIfValidMove() override;
};

class King : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() override;

	// bool checkIfValidMove() override;
};

#endif