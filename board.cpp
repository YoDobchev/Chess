#include "Board.h"
#include "Pieces.h"

Square::Square() : piece(nullptr), specialColor(0) {}
Square::~Square() { piece = nullptr; }

int Square::getSpecialColor() const { return specialColor; }

void Square::setSpecialColor(const int color) { specialColor = color; }

void Square::setPiece(Piece* piece) { this->piece = piece; }

bool Square::getAttackedBy(Player player) const { return isAttackedBy[static_cast<int>(player)]; }

void Square::setAttackedBy(Player player) { isAttackedBy[static_cast<int>(player)] = true; }

void Square::clearAttackedBy() {
	for (int i = 0; i < 2; ++i) {
		isAttackedBy[i] = false;
	}
}

Piece* Square::getPiece() const { return piece; }

Board::Board() : enPassantSquare({-1, -1}), checkExists(-1) {
	for (int i = 1; i < 8; ++i) {
		// board[1][i].setPiece(new Pawn(Player::WHITE, {1, i}));
		// board[6][i].setPiece(new Pawn(Player::BLACK, {6, i}));
	}

	Player players[2] = {Player::WHITE, Player::BLACK};
	int backRanks[2] = {0, 7};

	board[1][4].setPiece(new King(Player::WHITE, {4, 1}));
	// board[4][2].setPiece(new Pawn(Player::BLACK, {3, 2}));

	board[0][0].setPiece(new King(Player::BLACK, {0, 0}));
	// board[4][0].setPiece(new Rook(Player::BLACK, {4, 0}));
	// board[2][0].setPiece(new Rook(Player::BLACK, {2, 0}));
	board[1][0].setPiece(new Bishop(Player::BLACK, {1, 0}));
	board[0][1].setPiece(new Bishop(Player::BLACK, {0, 1}));
	board[4][0].setPiece(new Bishop(Player::BLACK, {4, 0}));

	for (int p = 0; p < 2; ++p) {
		int row = backRanks[p];
		Player color = players[p];
		// board[row][0].setPiece(new Rook(color, {row, 0}));
		// board[row][1].setPiece(new Knight(color, {row, 1}));
		// board[row][2].setPiece(new Bishop(color, {row, 2}));
		// board[row][3].setPiece(new Queen(color, {row, 3}));
		// board[row][4].setPiece(new King(color, {row, 4}));
		// board[row][5].setPiece(new Bishop(color, {row, 5}));
		// board[row][6].setPiece(new Knight(color, {row, 6}));
		// board[row][7].setPiece(new Rook(color, {row, 7}));
	}
}

Square* Board::operator[](const int row) { return board[row]; }

Piece* Board::getPieceAtPos(Position pos) const {
	if (pos.isOutOfBounds()) return nullptr;
	return board[pos.row][pos.col].getPiece();
}

void Board::setCheckExists(int exists) { checkExists = exists; }
int Board::getCheckExists() const { return checkExists; }

void Board::calculateSquares() {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			Piece* piece = board[i][j].getPiece();
			if (!piece) continue;

			piece->calculateValidMoves(this);
			piece->setAttackedSquares(this);
		}
	}

	// Calculate valid moves for both kings again
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			Piece* piece = board[i][j].getPiece();
			if (!piece) continue;

			if (King* king = dynamic_cast<King*>(piece)) {
				king->calculateValidMoves(this);
			}
		}
	}
}

void Board::clearAttackedSquares() {
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			board[i][j].clearAttackedBy();
		}
	}
}

bool Board::movePiece(const Position from, const Position to, const Player playerTurn, String& error) {
	calculateSquares();

	Piece* pieceToMove = getPieceAtPos(from);
	if (!pieceToMove) {
		error = "No piece at the from position!";
		return false;
	}

	if (checkExists == static_cast<int>(playerTurn) && dynamic_cast<King*>(pieceToMove) == nullptr) {
		error = "You cannot move a piece while in check!";
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

	setCheckExists(-1);

	clearAttackedSquares();

	return true;
}