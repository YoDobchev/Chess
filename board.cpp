#include "Board.h"
#include "Pieces.h"

Square::Square() : piece(nullptr), specialColor("") {}

String Square::getSpecialColor() const { return specialColor; }

void Square::setSpecialColor(const String& color) { specialColor = color; }

void Square::setPiece(Piece* piece) { this->piece = piece; }

Piece* Square::getPiece() const { return piece; }

Board::Board() : enPassantSquare({-1, -1}) {
	for (int i = 1; i < 8; ++i) {
		board[1][i].setPiece(new Pawn(Player::WHITE, {1, i}));
		board[6][i].setPiece(new Pawn(Player::BLACK, {6, i}));
	}

	Player players[2] = {Player::WHITE, Player::BLACK};
	int backRanks[2] = {0, 7};

	board[4][2].setPiece(new Pawn(Player::WHITE, {4, 2}));
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

Piece* Board::getPieceAtPos(Position pos) const { return board[pos.row][pos.col].getPiece(); }

bool Board::movePiece(const Position from, const Position to, const Player playerTurn, String& error) {
	std::cout << enPassantSquare.row << " " << enPassantSquare.col << "\n";

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			if (board[i][j].getPiece()) {
				board[i][j].getPiece()->calculateValidMoves(this);
			}
		}
	}

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

	if (!pieceToMove->checkIfValidMove(to, this, error)) {
		error = "Invalid move spot!";
		return false;
	}

	oldEnPassantSquare = enPassantSquare;
	if (enPassantSquare.row != -1) enPassantSquare = {-1, -1};

	pieceToMove->move(to, this, error);

	return true;
}