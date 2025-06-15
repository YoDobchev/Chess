#include "Board.h"
#include "Pieces.h"

Square::Square() : piece(nullptr), specialColor(0) {
	isAttackedBy[0] = false;
	isAttackedBy[1] = false;
}

Square::~Square() { delete piece; }

int Square::getSpecialColor() const { return specialColor; }

void Square::setSpecialColor(const int color) { specialColor = color; }

void Square::setPiece(Piece* piece) {
	if (this->piece != nullptr && piece != nullptr) delete this->piece;
	this->piece = piece;
}

bool Square::getAttackedBy(Player player) const { return isAttackedBy[static_cast<int>(player)]; }

void Square::setAttackedBy(Player player) { isAttackedBy[static_cast<int>(player)] = true; }

void Square::clearAttackedBy() {
	for (int i = 0; i < 2; ++i) {
		isAttackedBy[i] = false;
	}
}

Piece* Square::getPiece() const { return piece; }

Board::Board() : enPassantSquare({-1, -1}), checkExists(-1) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		setPiece(new Pawn(Player::WHITE), {1, i});
		setPiece(new Pawn(Player::BLACK), {6, i});
	}

	Player players[2] = {Player::WHITE, Player::BLACK};
	int backRanks[2] = {0, 7};

	for (int p = 0; p < 2; ++p) {
		int row = backRanks[p];
		Player color = players[p];
		setPiece(new Rook(color), {row, 0});
		setPiece(new Knight(color), {row, 1});
		setPiece(new Bishop(color), {row, 2});
		setPiece(new Queen(color), {row, 3});
		setPiece(new King(color), {row, 4});
		setPiece(new Bishop(color), {row, 5});
		setPiece(new Knight(color), {row, 6});
		setPiece(new Rook(color), {row, 7});
	}
	calculateSquares();
}

Board::Board(const String& serializedData) : enPassantSquare({-1, -1}), oldEnPassantSquare({-1, -1}), checkExists(-1) {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			char pieceChar = serializedData[i * 9 + j];
			if (pieceChar == '\0' || pieceChar == '\n') continue;
			if (pieceChar != '*') {
				Player color = (pieceChar >= 'a' && pieceChar <= 'z') ? Player::WHITE : Player::BLACK;
				Position pos = {i, j};
				switch (Utility::toLower(pieceChar)) {
				case 'p':
					setPiece(new Pawn(color), pos);
					break;
				case 'r':
					setPiece(new Rook(color), pos);
					break;
				case 'b':
					setPiece(new Bishop(color), pos);
					break;
				case 'n':
					setPiece(new Knight(color), pos);
					break;
				case 'q':
					setPiece(new Queen(color), pos);
					break;
				case 'k':
					setPiece(new King(color), pos);
					break;
				}
			}
		}
	}
}

Square* Board::operator[](const int row) { return board[row]; }

String Board::serialize() const {
	String result;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			Piece* piece = board[i][j].getPiece();
			if (piece) {
				result.push_back(piece->serialize());
			} else {
				result.push_back('*');
			}
		}
		result.push_back('\n');
	}
	return result;
}

void Board::setEnPassantSquare(const Position pos) { enPassantSquare = pos; }

Position Board::getEnPassantSquare() const { return enPassantSquare; }
Position Board::getOldEnPassantSquare() const { return oldEnPassantSquare; }

Vector<Position>& Board::getPositionsToBlockCheck() { return positionsToBlockCheck; }

void Board::addToPositionsToBlockCheck(const Position pos) {
	if (pos.isOutOfBounds()) return;
	positionsToBlockCheck.push_back(pos);
}

void Board::setPiece(Piece* piece, const Position pos) {
	if (pos.isOutOfBounds()) return;
	board[pos.row][pos.col].setPiece(piece);
	if (piece) piece->setPosition(pos);
}

Piece* Board::getPieceAtPos(Position pos) const {
	if (pos.isOutOfBounds()) return nullptr;
	return board[pos.row][pos.col].getPiece();
}

void Board::setCheckExists(int exists) { checkExists = exists; }
int Board::getCheckExists() const { return checkExists; }

void Board::calculateSquares() {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			Piece* piece = board[i][j].getPiece();
			if (!piece) continue;

			piece->setIsPinned(false);
			piece->updateMoves(this);
			piece->setAttackedSquares(this);
			if (checkExists != -1)
				piece->removeValidMovesThatDoNotProtectKing(this->positionsToBlockCheck, this->checkExists);
		}
	}

	// Calculate valid moves for both kings again & update valid moves based on pinned
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			Piece* piece = board[i][j].getPiece();
			if (!piece) continue;

			piece->removeValidMovesIfPinned();

			if (King* king = dynamic_cast<King*>(piece)) {
				king->updateMoves(this);
			}
		}
	}
}

void Board::clearAttackedSquares() {
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			board[i][j].clearAttackedBy();
		}
	}
}

bool Board::movePiece(const Position from, const Position to, const Player playerTurn, String& error) {
	Piece* pieceToMove = getPieceAtPos(from);
	if (!pieceToMove) {
		error = "No piece at the from position!";
		return false;
	}

	if (pieceToMove->getColor() != playerTurn) {
		error = "It's not your turn!";
		return false;
	}

	if (!pieceToMove->checkIfValidMove(to, this)) {
		error = "Invalid move spot!";
		return false;
	}

	Piece* targetPiece = getPieceAtPos(to);
	if (targetPiece && targetPiece->getColor() == playerTurn) {
		error = "You cannot capture your own piece!";
		return false;
	}

	oldEnPassantSquare = enPassantSquare;
	if (enPassantSquare.row != -1) setEnPassantSquare({-1, -1});

	pieceToMove->move(to, this);

	setCheckExists(NO_CHECK);
	positionsToBlockCheck.clear();

	clearAttackedSquares();

	return true;
}