#include "Board.h"
#include "Pieces.h"

Square::Square() : piece(nullptr) {}

// Square::~Square() { delete piece; }

void Square::setPiece(Piece* piece) { this->piece = piece; }

Piece* Square::getPiece() const { return piece; }

Board::Board() {
	for (int i = 0; i < 8; ++i) {
		board[1][i].setPiece(new Pawn(Player::WHITE));
		board[6][i].setPiece(new Pawn(Player::BLACK));
	}

	Player players[2] = {Player::WHITE, Player::BLACK};
	int backRanks[2] = {0, 7};

	for (int p = 0; p < 2; ++p) {
		int row = backRanks[p];
		Player color = players[p];
		board[row][0].setPiece(new Rook(color));
		board[row][1].setPiece(new Knight(color));
		board[row][2].setPiece(new Bishop(color));
		board[row][3].setPiece(new Queen(color));
		board[row][4].setPiece(new King(color));
		board[row][5].setPiece(new Bishop(color));
		board[row][6].setPiece(new Knight(color));
		board[row][7].setPiece(new Rook(color));
	}
}

Square* Board::operator[](const int row) { return board[row]; }

Piece* Board::getPieceAtPos(Position pos) const {
	// if (!board[pos.row][pos.col]) return nullptr;
	return board[pos.row][pos.col].getPiece();
}