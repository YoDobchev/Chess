#include "Pieces.h"
#include "Board.h"
// #include "Types.h"

Piece::Piece(Player color, Position pos) : color(color), pos_(pos) {}

Player Piece::getColor() const { return color; }

String Pawn::getEmoji() const { return color == Player::WHITE ? "♙" : "♟"; }

String Rook::getEmoji() const { return color == Player::WHITE ? "♖" : "♜"; }

String Bishop::getEmoji() const { return color == Player::WHITE ? "♗" : "♝"; }

String Knight::getEmoji() const { return color == Player::WHITE ? "♘" : "♞"; }

String Queen::getEmoji() const { return color == Player::WHITE ? "♕" : "♛"; }

String King::getEmoji() const { return color == Player::WHITE ? "♔" : "♚"; }

bool Pawn::checkIfValidMove(const Position to, const Board* board, String& error) const {
    return true;
}

bool Rook::checkIfValidMove(const Position to, const Board* board, String& error) const {
    return true;
}

bool Bishop::checkIfValidMove(const Position to, const Board* board, String& error) const {
    return true;
}

bool Knight::checkIfValidMove(const Position to, const Board* board, String& error) const {
    return true;
}

bool Queen::checkIfValidMove(const Position to, const Board* board, String& error) const {
    return true;
}

bool King::checkIfValidMove(const Position to, const Board* board, String& error) const {
    // Implement king movement logic
    return true;
}