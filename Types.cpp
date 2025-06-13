#include "Types.h"

Player operator!(Player p) { return (p == Player::WHITE) ? Player::BLACK : Player::WHITE; }

std::ostream& operator<<(std::ostream& os, const Player& player) {
	return os << (player == Player::WHITE ? "WHITE" : "BLACK");
}
