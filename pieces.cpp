#include "Pieces.h"
#include "Board.h"
#include "String.h"
#include "Types.h"

Piece::Piece(Player color, Position pos) : color(color), pos(pos), hasMoved(false) {}

Player Piece::getColor() const { return color; }

void Piece::setPosition(Position newPos) { pos = newPos; }

bool Piece::checkIfValidMove(const Position to, const Board* board, String& error) {
	for (int i = 0; i < validMoves.size(); ++i) {
		if (validMoves[i] == to) return true;
	}
	return false;
}

void Piece::addValidMovesBasedOnDirections(const Board* board, const Vector<Direction>& directions) {
	validMoves.clear();
	for (int i = 0; i < directions.size(); ++i) {
		Position next = pos;
		while (true) {
			next.move(directions[i]);
			if (next.isOutOfBounds()) break;

			Piece* targetPiece = board->getPieceAtPos(next);
			if (targetPiece == nullptr) {
				validMoves.push_back(next);
			} else {
				if (targetPiece->getColor() != color) {
					validMoves.push_back(next);
				}
				break;
			}
		}
	}
}

String Pawn::getEmoji() const { return color == Player::WHITE ? "♙" : "♟"; }

String Rook::getEmoji() const { return color == Player::WHITE ? "♖" : "♜"; }

String Bishop::getEmoji() const { return color == Player::WHITE ? "♗" : "♝"; }

String Knight::getEmoji() const { return color == Player::WHITE ? "♘" : "♞"; }

String Queen::getEmoji() const { return color == Player::WHITE ? "♕" : "♛"; }

String King::getEmoji() const { return color == Player::WHITE ? "♔" : "♚"; }

void Pawn::calculateValidMoves(const Board* board) {
	validMoves.clear();
	const int direction = (color == Player::WHITE) ? 1 : -1;
	const int startRow = (color == Player::WHITE) ? 1 : 6;

	Position forwardPos = {pos.row + direction, pos.col};
	if (board->getPieceAtPos(forwardPos) == nullptr) {
		validMoves.push_back(forwardPos);
	}

	if (pos.row == startRow) {
		Position doubleForwardPos = {pos.row + 2 * direction, pos.col};
		if (board->getPieceAtPos(doubleForwardPos) == nullptr && board->getPieceAtPos(forwardPos) == nullptr) {
			validMoves.push_back(doubleForwardPos);
		}
	}

	const int colOffsets[2] = {-1, 1};
	for (int i = 0; i < 2; ++i) {
		Position capturePos = {pos.row + direction, pos.col + colOffsets[i]};
		Piece* targetPiece = board->getPieceAtPos(capturePos);
		if (targetPiece && targetPiece->getColor() != color) {
			validMoves.push_back(capturePos);
		}
		if (capturePos == board->enPassantSquare) {
			// if (board->getPieceAtPos(enPassantCapturePos) == nullptr) {
			validMoves.push_back(capturePos);
			// }
		}
	}
}

void Rook::calculateValidMoves(const Board* board) {
	validMoves.clear();
	Vector<Direction> directions;
	directions.push_back(UP);
	directions.push_back(DOWN);
	directions.push_back(LEFT);
	directions.push_back(RIGHT);
	addValidMovesBasedOnDirections(board, directions);
}

void Bishop::calculateValidMoves(const Board* board) {
	validMoves.clear();
	Vector<Direction> directions;
	directions.push_back(UP_LEFT);
	directions.push_back(UP_RIGHT);
	directions.push_back(DOWN_LEFT);
	directions.push_back(DOWN_RIGHT);
	addValidMovesBasedOnDirections(board, directions);
}

void Knight::calculateValidMoves(const Board* board) {
	validMoves.clear();
	const Direction knightMoves[] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

	for (int i = 0; i < 8; ++i) {
		Position next = pos;
		next.move(knightMoves[i]);
		if (!next.isOutOfBounds()) {
			Piece* targetPiece = board->getPieceAtPos(next);
			if (targetPiece == nullptr || targetPiece->getColor() != color) {
				validMoves.push_back(next);
			}
		}
	}
}

void Queen::calculateValidMoves(const Board* board) {
	validMoves.clear();
	Vector<Direction> directions;
	directions.push_back(UP);
	directions.push_back(DOWN);
	directions.push_back(LEFT);
	directions.push_back(RIGHT);
	directions.push_back(UP_LEFT);
	directions.push_back(UP_RIGHT);
	directions.push_back(DOWN_LEFT);
	directions.push_back(DOWN_RIGHT);
	addValidMovesBasedOnDirections(board, directions);
}

void King::calculateValidMoves(const Board* board) {
	validMoves.clear();
	const Direction kingMoves[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	for (int i = 0; i < 8; ++i) {
		Position next = pos;
		next.move(kingMoves[i]);
		if (!next.isOutOfBounds()) {
			Piece* targetPiece = board->getPieceAtPos(next);
			if (targetPiece == nullptr || targetPiece->getColor() != color) {
				validMoves.push_back(next);
			}
		}
	}
}

void Piece::move(const Position to, Board* board, String& error) {
	Position from = pos;
	this->setPosition(to);
	board->operator[](to.row)[to.col].setPiece(this);
	board->operator[](from.row)[from.col].setPiece(nullptr);
	hasMoved = true;
}

void Pawn::move(const Position to, Board* board, String& error) {
	const int direction = (color == Player::WHITE) ? 1 : -1;
	if (to.row == pos.row + direction * 2) {
		board->enPassantSquare = {pos.row + direction, pos.col};
	}

	const int colOffsets[2] = {-1, 1};
	for (int i = 0; i < 2; ++i) {
		Position capturePos = {pos.row + direction, pos.col + colOffsets[i]};
		if (capturePos == board->enPassantSquare) {
			Position enPassantCapturePos = {to.row - direction, to.col};
			board->operator[](enPassantCapturePos.row)[enPassantCapturePos.col].setPiece(nullptr);
		}
	}

	Piece::move(to, board, error);
}