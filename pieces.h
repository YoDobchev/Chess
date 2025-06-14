#ifndef PIECES_H
#define PIECES_H

#include "Types.h"
#include "String.h"
#include "Vector.h"

class Board;

class Piece {
  protected:
	Player color;
	Position pos;
	Vector<Position> validMoves, attackingMoves;
	bool hasMoved;

	void addValidMovesBasedOnDirections(const Board* board, const Vector<Direction>& directions);

  public:
	Piece(Player color);
	virtual ~Piece() = default;
	Player getColor() const;
	const Vector<Position>& getValidMoves() const;
	bool getHasMoved() const;
	void setHasMoved(bool moved);
	void setPosition(Position newPos);
	virtual String getEmoji() const = 0;
	virtual void calculateValidMoves(Board* board) = 0;
	virtual char serialize() const = 0;
	bool checkIfValidMove(const Position to, const Board* board, String& error);
	virtual void setAttackedSquares(Board* board) const;
	virtual void move(const Position to, Board* board, String& error);
};

class Pawn : public Piece {
  private:
  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	void calculateValidMoves(Board* board) override;
	void setAttackedSquares(Board* board) const override;
	void move(const Position to, Board* board, String& error) override;
	// bool checkIfValidMove(const Position to, const Board* board, String& error) override;
};

class Rook : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	void calculateValidMoves(Board* board) override;
	// void move(const Position to, Board* board, String& error) override;
	using Piece::move;
	using Piece::setAttackedSquares;
	// bool checkIfValidMove(const Position to, const Board* board, String& error) override;
};

class Bishop : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	void calculateValidMoves(Board* board) override;
	// void move(const Position to, Board* board, String& error) override;
	using Piece::move;
	using Piece::setAttackedSquares;
	// bool checkIfValidMove(const Position to, const Board* board, String& error) override;
};

class Knight : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	void calculateValidMoves(Board* board) override;
	// void move(const Position to, Board* board, String& error) override;
	using Piece::move;
	using Piece::setAttackedSquares;
	// bool checkIfValidMove(const Position to, const Board* board, String& error) override;
};

class Queen : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	void calculateValidMoves(Board* board) override;
	// void move(const Position to, Board* board, String& error) override;
	using Piece::move;
	using Piece::setAttackedSquares;
	// bool checkIfValidMove(const Position to, const Board* board, String& error) override;
};

class King : public Piece {
  private:
	bool canLongCastle, canShortCastle;
	static void castleRook(Board* board, int row, int fromCol, int toCol);

  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	void calculateValidMoves(Board* board) override;
	void setAttackedSquares(Board* board) const override;
	void move(const Position to, Board* board, String& error) override;
	// bool checkIfValidMove(const Position to, const Board* board, String& error) override;
};

#endif