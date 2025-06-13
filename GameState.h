#include <iostream>
#include "Board.h"
#include "String.h"
#include "InputHandler.h"
#include "Types.h"

class GameState {
  private:
	bool gameOver;
	Player playerTurn;
	Board* board;
	String error;
  public:

	GameState();
	~GameState();
	void printBoard();
	void update();
	void start();
};