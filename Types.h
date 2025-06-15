#ifndef TYPES_H
#define TYPES_H
#include <iostream>

const int BOARD_SIZE = 8;
const char BOARD_SIZE_CHAR = '8';

struct Direction {
	int dRow;
	int dCol;
};

const Direction UP = {-1, 0};
const Direction DOWN = {1, 0};
const Direction LEFT = {0, -1};
const Direction RIGHT = {0, 1};

const Direction UP_LEFT = {-1, -1};
const Direction UP_RIGHT = {-1, 1};
const Direction DOWN_LEFT = {1, -1};
const Direction DOWN_RIGHT = {1, 1};

struct Position {
	int row, col;
	bool operator==(const Position& other) const;
	bool operator!=(const Position& other) const;
	Position& move(Direction dir);
	bool isOutOfBounds() const;
};

const int NO_CHECK = -1;

enum class Player { WHITE, BLACK };

Player operator!(Player p);

std::ostream& operator<<(std::ostream& os, const Player& player);

#endif
