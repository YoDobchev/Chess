#include "Pieces.h"
#include "Board.h"

Piece::Piece(Player color) : color(color), hasMoved(false) {}

Player Piece::getColor() const { return color; }

const Vector<Position>& Piece::getValidMoves() const { return validMoves; }

bool Piece::getHasMoved() const { return hasMoved; }

void Piece::setHasMoved(bool moved) { hasMoved = moved; }

void Piece::clearPinnedPositions() { validMovesWhenPinned.clear(); }

void Piece::setPosition(Position newPos) { pos = newPos; }

bool Piece::checkIfValidMove(const Position to, const Board* board) {
	for (int i = 0; i < validMoves.size(); ++i) {
		if (validMoves[i] == to) return true;
	}
	return false;
}

void Piece::removeValidMovesThatDoNotProtectKing(const Vector<Position>& positionsThatCanBlock, const int checkExists) {
	if (static_cast<int>(color) != (checkExists)) return;

	Vector<Position> filtered;
	for (int i = 0; i < validMoves.size(); ++i) {
		for (int j = 0; j < positionsThatCanBlock.size(); ++j) {
			if (validMoves[i] == positionsThatCanBlock[j]) {
				filtered.push_back(validMoves[i]);
				break;
			}
		}
	}

	validMoves = std::move(filtered);
}

void Piece::setAttackedSquares(Board* board) const {
	for (int i = 0; i < attackingMoves.size(); ++i) {
		Position movePos = attackingMoves[i];
		if (movePos.isOutOfBounds()) continue;
		(*board)[movePos.row][movePos.col].setAttackedBy(color);
		King* king = dynamic_cast<King*>(board->getPieceAtPos(movePos));
		if (king && king->getColor() != color) {
			board->setCheckExists(static_cast<int>(!color));
		}
	}
}

void Pawn::setAttackedSquares(Board* board) const {
	const int direction = (color == Player::WHITE) ? 1 : -1;
	const int colOffsets[2] = {-1, 1};

	for (int i = 0; i < 2; ++i) {
		Position capturePos = {pos.row + direction, pos.col + colOffsets[i]};
		if (capturePos.isOutOfBounds()) continue;
		Piece* targetPiece = board->getPieceAtPos(capturePos);
		(*board)[capturePos.row][capturePos.col].setAttackedBy(color);
		King* king = dynamic_cast<King*>(targetPiece);
		if (king && king->getColor() != color) {
			board->setCheckExists(static_cast<int>(!color));
		}
	}
}

void King::setAttackedSquares(Board* board) const {
	const Direction kingMoves[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	for (int i = 0; i < BOARD_SIZE; ++i) {
		Position next = pos;
		next.move(kingMoves[i]);
		if (!next.isOutOfBounds()) {
			(*board)[next.row][next.col].setAttackedBy(color);
		}
	}
}

void Pawn::calculateMoves(Board* board) {
	validMoves.clear();
	attackingMoves.clear();
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
		if (capturePos.isOutOfBounds()) continue;
		Piece* targetPiece = board->getPieceAtPos(capturePos);
		if (targetPiece && targetPiece->getColor() != color) {
			validMoves.push_back(capturePos);
		}
		if (capturePos == board->getEnPassantSquare() && pos.row != startRow) {
			validMoves.push_back(capturePos);
		}
	}
}

void Piece::detectIfMoveAllowedInDirections(Board* board, const Vector<Direction>& directions) {
	for (int i = 0; i < directions.size(); ++i) {
		Vector<Position> positionsInThisDir;
		Position next = pos;
		bool sawEnemyKing = false;
		while (true) {
			// Get up to king
			next.move(directions[i]);
			if (next.isOutOfBounds()) break;
			positionsInThisDir.push_back(next);
			Piece* targetPiece = board->getPieceAtPos(next);
			if (targetPiece == nullptr) continue;
			King* king = dynamic_cast<King*>(targetPiece);
			if (king != nullptr && king->getColor() != color) {
				sawEnemyKing = true;
				break;
			};
		}

		if (!sawEnemyKing) continue;

		int piecesBetweenAttackerAndKing = 0, pieceIndexInVector;
		Piece* pieceInBetween = nullptr;
		for (int i = 0; i < positionsInThisDir.size(); ++i) {
			Piece* piece = board->getPieceAtPos(positionsInThisDir[i]);

			if (!piece) continue;

			if (dynamic_cast<King*>(piece) != nullptr) break;
			++piecesBetweenAttackerAndKing;
			if (piecesBetweenAttackerAndKing == 1) {
				pieceInBetween = piece;
				pieceIndexInVector = i;
			} else {
				pieceInBetween = nullptr;
				break;
			}
		}

		// Prevent the piece from attacking itself and the king
		positionsInThisDir.erase(pieceIndexInVector);
		positionsInThisDir.erase(positionsInThisDir.size() - 1);
		positionsInThisDir.push_back(pos);

		if (piecesBetweenAttackerAndKing == 1 && pieceInBetween) {
			pieceInBetween->validMovesWhenPinned = std::move(positionsInThisDir);
		}
		break;
	}
}

void Piece::addValidMovesBasedOnDirections(Board* board, const Vector<Direction>& directions) {
	for (int i = 0; i < directions.size(); ++i) {
		bool sawEnemyKing = false;
		Vector<Position> positionsInThisDir;
		Position next = pos;
		// Add the position of the piece that is attacking to the possible defences
		positionsInThisDir.push_back(pos);
		while (true) {
			next.move(directions[i]);
			if (next.isOutOfBounds()) break;

			positionsInThisDir.push_back(next);
			Piece* targetPiece = board->getPieceAtPos(next);
			if (targetPiece == nullptr) {
				validMoves.push_back(next);
				attackingMoves.push_back(next);
				continue;
			}

			if (targetPiece->getColor() != color) {
				validMoves.push_back(next);
			} else {
				attackingMoves.push_back(next);
			}

			King* king = dynamic_cast<King*>(targetPiece);
			if (king != nullptr && king->getColor() != color) {
				attackingMoves.push_back(next);
				sawEnemyKing = true;

				Position oneOver = next;
				oneOver.move(directions[i]);
				if (!oneOver.isOutOfBounds()) {
					attackingMoves.push_back(oneOver);
				}
			}

			break;
		}

		if (sawEnemyKing) {
			for (int i = 0; i < positionsInThisDir.size() - 1; ++i) {
				board->addToPositionsToBlockCheck(positionsInThisDir[i]);
			}
		}

		positionsInThisDir.clear();
	}

	detectIfMoveAllowedInDirections(board, directions);
}

void Rook::calculateMoves(Board* board) {
	validMoves.clear();
	attackingMoves.clear();
	Vector<Direction> directions;
	directions.push_back(UP);
	directions.push_back(DOWN);
	directions.push_back(LEFT);
	directions.push_back(RIGHT);
	addValidMovesBasedOnDirections(board, directions);
}

void Bishop::calculateMoves(Board* board) {
	validMoves.clear();
	attackingMoves.clear();
	Vector<Direction> directions;
	directions.push_back(UP_LEFT);
	directions.push_back(UP_RIGHT);
	directions.push_back(DOWN_LEFT);
	directions.push_back(DOWN_RIGHT);
	addValidMovesBasedOnDirections(board, directions);
}

void Knight::calculateMoves(Board* board) {
	validMoves.clear();
	attackingMoves.clear();
	const Direction knightMoves[] = {{2, 1}, {2, -1}, {-2, 1}, {-2, -1}, {1, 2}, {1, -2}, {-1, 2}, {-1, -2}};

	for (int i = 0; i < BOARD_SIZE; ++i) {
		Position next = pos;
		next.move(knightMoves[i]);
		if (!next.isOutOfBounds()) {
			Piece* targetPiece = board->getPieceAtPos(next);
			if (targetPiece == nullptr || targetPiece->getColor() != color) {
				validMoves.push_back(next);
				attackingMoves.push_back(next);
			}
		}
	}
}

void Queen::calculateMoves(Board* board) {
	validMoves.clear();
	attackingMoves.clear();
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

static bool pathClear(Board* board, int row, int c1, int c2) {
	for (int c = c1; c <= c2; ++c)
		if (board->getPieceAtPos({row, c})) return false;
	return true;
}

bool King::canCastleLong(Board* board, int backrank) const {
	Piece* rook = board->getPieceAtPos({backrank, 0});
	if (!rook || rook->getHasMoved()) return false;

	if (!pathClear(board, backrank, 1, 3)) return false;

	Player opp = !color;
	if ((*board)[backrank][3].getAttackedBy(opp) || (*board)[backrank][2].getAttackedBy(opp)) return false;

	return true;
}

bool King::canCastleShort(Board* board, int backrank) const {
	Piece* rook = board->getPieceAtPos({backrank, 7});
	if (!rook || rook->getHasMoved()) return false;

	if (!pathClear(board, backrank, 5, 6)) return false;

	Player opp = !color;
	if ((*board)[backrank][5].getAttackedBy(opp) || (*board)[backrank][6].getAttackedBy(opp)) return false;

	return true;
}

void King::calculateMoves(Board* board) {
	validMoves.clear();
	attackingMoves.clear();
	const Direction kingMoves[] = {{0, 1}, {0, -1}, {1, 0}, {-1, 0}, {1, 1}, {1, -1}, {-1, 1}, {-1, -1}};

	for (int i = 0; i < BOARD_SIZE; ++i) {
		Position next = pos;
		next.move(kingMoves[i]);
		if (!next.isOutOfBounds()) {
			Piece* targetPiece = board->getPieceAtPos(next);
			if (targetPiece == nullptr || targetPiece->getColor() != color) {
				validMoves.push_back(next);
				attackingMoves.push_back(next);
			}
		}
	}

	canShortCastle = false;
	canLongCastle = false;
	const int backrank = (color == Player::WHITE) ? 0 : 7;
	if (pos.row == backrank && pos.col == 4 && !hasMoved && board->getCheckExists() != static_cast<int>(color)) {
		if (canCastleLong(board, backrank)) {
			validMoves.push_back({backrank, 2});
			attackingMoves.push_back({backrank, 2});
			canLongCastle = true;
		}
		if (canCastleShort(board, backrank)) {
			validMoves.push_back({backrank, 6});
			attackingMoves.push_back({backrank, 6});
			canShortCastle = true;
		}
	}

	Player opponent = !color;
	for (int i = 0; i < validMoves.size();) {
		Position dest = validMoves[i];
		bool attacked = (*board)[dest.row][dest.col].getAttackedBy(opponent);

		if (attacked) {
			validMoves.erase(i);
		} else {
			++i;
		}
	}
}

void Piece::removeValidMovesIfPinned() {
	if (!validMovesWhenPinned.empty()) {
		for (int i = 0; i < validMoves.size();) {
			bool isValidAfterFilter = false;
			for (int j = 0; j < validMovesWhenPinned.size(); j++) {
				if (validMoves[i] == validMovesWhenPinned[j]) isValidAfterFilter = true;
			}

			if (!isValidAfterFilter) {
				validMoves.erase(i);
			} else {
				++i;
			}
		}
	}
}

void Piece::move(const Position to, Board* board) {
	Position from = pos;
	this->setPosition(to);
	(*board)[to.row][to.col].setPiece(this);
	(*board)[from.row][from.col].setPiece(nullptr);
	hasMoved = true;
}

void Pawn::move(const Position to, Board* board) {
	const int direction = (color == Player::WHITE) ? 1 : -1;
	if (to.row == pos.row + direction * 2) {
		board->setEnPassantSquare({pos.row + direction, pos.col});
	}

	const int colOffsets[2] = {-1, 1};
	for (int i = 0; i < 2; ++i) {
		Position capturePos = {pos.row + direction, pos.col + colOffsets[i]};
		if (capturePos.isOutOfBounds()) continue;
		if (capturePos == board->getOldEnPassantSquare()) {
			Position enPassantCapturePos = {to.row - direction, to.col};
			delete board->getPieceAtPos(enPassantCapturePos);
			(*board)[enPassantCapturePos.row][enPassantCapturePos.col].setPiece(nullptr);
		}
	}

	Piece::move(to, board);
}

void King::castleRook(Board* board, int row, int fromCol, int toCol) {
	Position from{row, fromCol};
	Position to{row, toCol};
	Piece* rook = board->getPieceAtPos(from);
	if (!rook) return;
	rook->setPosition(to);
	(*board)[to.row][to.col].setPiece(rook);
	(*board)[from.row][from.col].setPiece(nullptr);
	rook->setHasMoved(true);
}

void King::move(const Position to, Board* board) {
	bool isShort = canShortCastle && to == Position{pos.row, pos.col + 2};
	bool isLong = canLongCastle && to == Position{pos.row, pos.col - 2};
	if (isShort) {
		castleRook(board, to.row, pos.col + 3, pos.col + 1);
	} else if (isLong) {
		castleRook(board, to.row, pos.col - 4, pos.col - 1);
	}
	Piece::move(to, board);
}

char Pawn::serialize() const { return (color == Player::WHITE) ? 'p' : 'P'; }
char Rook::serialize() const { return (color == Player::WHITE) ? 'r' : 'R'; }
char Bishop::serialize() const { return (color == Player::WHITE) ? 'b' : 'B'; }
char Knight::serialize() const { return (color == Player::WHITE) ? 'n' : 'N'; }
char Queen::serialize() const { return (color == Player::WHITE) ? 'q' : 'Q'; }
char King::serialize() const { return (color == Player::WHITE) ? 'k' : 'K'; }

String Pawn::getEmoji() const { return color == Player::WHITE ? "♙" : "♟"; }
String Rook::getEmoji() const { return color == Player::WHITE ? "♖" : "♜"; }
String Bishop::getEmoji() const { return color == Player::WHITE ? "♗" : "♝"; }
String Knight::getEmoji() const { return color == Player::WHITE ? "♘" : "♞"; }
String Queen::getEmoji() const { return color == Player::WHITE ? "♕" : "♛"; }
String King::getEmoji() const { return color == Player::WHITE ? "♔" : "♚"; }
