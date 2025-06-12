#include <iostream>
#include "board.h"
#include "String.h"
#include "InputHandler.h"

class GameState {
  private:
	bool playerTurn;
	Board board;
  public:
	void printBoard(const Board& board);
	void update();
	void startGame();
};