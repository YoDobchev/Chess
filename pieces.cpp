#include "Pieces.h"

Piece::Piece(Player color) : color(color) {}

String Pawn::getEmoji() { return color == Player::WHITE ? "♙" : "♟"; }

String Rook::getEmoji() { return color == Player::WHITE ? "♖" : "♜"; }

String Bishop::getEmoji() { return color == Player::WHITE ? "♗" : "♝"; }

String Knight::getEmoji() { return color == Player::WHITE ? "♘" : "♞"; }


String Queen::getEmoji() { return color == Player::WHITE ? "♕" : "♛"; }

String King::getEmoji() { return color == Player::WHITE ? "♔" : "♚"; }