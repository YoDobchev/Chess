#include "Board.h"
#include "Pieces.h"
#include "String.h"
#include "Types.h"

Square::Square() : piece(nullptr) {}

// Square::~Square() { delete piece; }

void Square::setPiece(Piece* piece) { this->piece = piece; }

Piece* Square::getPiece() const { return piece; }

Board::Board() {
	for (int i = 0; i < 8; ++i) {
		board[1][i].setPiece(new Pawn(Player::WHITE, {1, i}));
		board[6][i].setPiece(new Pawn(Player::BLACK, {6, i}));
	}

	Player players[2] = {Player::WHITE, Player::BLACK};
	int backRanks[2] = {0, 7};

	for (int p = 0; p < 2; ++p) {
		int row = backRanks[p];
		Player color = players[p];
		board[row][0].setPiece(new Rook(color, {row, 0}));
		board[row][1].setPiece(new Knight(color, {row, 1}));
		board[row][2].setPiece(new Bishop(color, {row, 2}));
		board[row][3].setPiece(new Queen(color, {row, 3}));
		board[row][4].setPiece(new King(color, {row, 4}));
		board[row][5].setPiece(new Bishop(color, {row, 5}));
		board[row][6].setPiece(new Knight(color, {row, 6}));
		board[row][7].setPiece(new Rook(color, {row, 7}));
	}
}

Square* Board::operator[](const int row) { return board[row]; }

Piece* Board::getPieceAtPos(Position pos) const {
	// if (!board[pos.row][pos.col]) return nullptr;
	return board[pos.row][pos.col].getPiece();
}

bool Board::movePiece(const Position from, const Position to, const Player playerTurn, String& error) {
	Piece* pieceToMove = getPieceAtPos(from);
	if (!pieceToMove) {
		error = "No piece at the source position!";
		return false;
	}
	if (pieceToMove->getColor() != playerTurn) {
		error = "It's not your turn!";
		return false;
	}

	Piece* targetPiece = getPieceAtPos(to);
	if (targetPiece && targetPiece->getColor() == playerTurn) {
		error = "You cannot capture your own piece!";
		return false;
	}

	if (!pieceToMove->checkIfValidMove(to, this, error)) return false;
	return true;
}