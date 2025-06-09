// typedef struct {
//     int x, y; 
// } Position;
#ifndef PIECES_H
#define PIECES_H

#include "board.h"

class Piece {
    private:
    // Square* pos;
    bool color;
    public:
    virtual bool checkIfValidMove();
};

class Pawn: public Piece {
    public:
    bool checkIfValidMove() override;
};

class Rook: public Piece {
    public:
    bool checkIfValidMove() override;
};

class Bishop : public Piece {
    public:
    bool checkIfValidMove() override;
};

class Knight : public Piece {
    public:
    bool checkIfValidMove() override;
};

class Queen : public Piece {
    public:
    bool checkIfValidMove() override;
};

class Kign : public Piece {
    public:
    bool checkIfValidMove() override;
};

#endif