#include "Board.h"

Square* Board::operator[](int row) {
    if (row < 0 || row >= 8) {
        return nullptr;
    }
    return board[row];
}