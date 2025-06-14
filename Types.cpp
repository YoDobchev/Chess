#include "Types.h"

bool Position::operator==(const Position& other) const {
    return (this->row == other.row && this->col == other.col);
}

bool Position::operator!=(const Position& other) const {
    return !(*this == other);
}

Position& Position::move(Direction dir) {
    this->row += dir.dRow;
    this->col += dir.dCol;
    return *this;
}

bool Position::isOutOfBounds() const {
    return this->row < 0 || this->row >= 8 || this->col < 0 || this->col >= 8;
}

Player operator!(Player p) { return (p == Player::WHITE) ? Player::BLACK : Player::WHITE; }

std::ostream& operator<<(std::ostream& os, const Player& player) {
	return os << (player == Player::WHITE ? "WHITE" : "BLACK");
}
