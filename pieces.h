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
	Vector<Position> validMoves, attackingMoves, validMovesWhenPinned;
	bool hasMoved;

	void addValidMovesBasedOnDirections(Board* board, const Vector<Direction>& directions);
	void detectIfMoveAllowedInDirections(Board* board, const Vector<Direction>& directions);

  public:
	Piece(Player color);
	virtual ~Piece() = default;
	Player getColor() const;
	const Vector<Position>& getValidMoves() const;
	bool getHasMoved() const;
	void setHasMoved(bool moved);
	void clearPinnedPositions();
	void setPosition(Position newPos);
	virtual void calculateMoves(Board* board) = 0;
	void removeValidMovesIfPinned();
	bool checkIfValidMove(const Position to, const Board* board);
	void removeValidMovesThatDoNotProtectKing(const Vector<Position>& positionsThatCanBlock, const int checkExists);
	virtual void setAttackedSquares(Board* board) const;
	virtual void move(const Position to, Board* board);
	virtual char serialize() const = 0;
	virtual String getEmoji() const = 0;
};

class Pawn : public Piece {
  private:
  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	void calculateMoves(Board* board) override;
	void setAttackedSquares(Board* board) const override;
	void move(const Position to, Board* board) override;
};

class Rook : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	void calculateMoves(Board* board) override;
	using Piece::move;
	using Piece::setAttackedSquares;
};

class Bishop : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	void calculateMoves(Board* board) override;
	using Piece::move;
	using Piece::setAttackedSquares;
};

class Knight : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	void calculateMoves(Board* board) override;
	using Piece::move;
	using Piece::setAttackedSquares;
};

class Queen : public Piece {
  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	void calculateMoves(Board* board) override;
	using Piece::move;
	using Piece::setAttackedSquares;
};

class King : public Piece {
  private:
	bool canLongCastle, canShortCastle;
	static void castleRook(Board* board, int row, int fromCol, int toCol);

  public:
	using Piece::Piece;
	String getEmoji() const override;
	char serialize() const override;

	bool canCastleLong(Board* board, int backrank) const;
	bool canCastleShort(Board* board, int backrank) const;

	void calculateMoves(Board* board) override;
	void setAttackedSquares(Board* board) const override;
	void move(const Position to, Board* board) override;
};

#endif