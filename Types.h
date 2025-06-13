#ifndef TYPES_H
#define TYPES_H
#include <iostream>

struct Position {
	int row, col;
};

enum class Player {
	WHITE,
	BLACK	
};

Player operator!(Player p);

std::ostream& operator<<(std::ostream& os, const Player& player);

#endif
