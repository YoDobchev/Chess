#include <iostream>
#include "Board.h"
#include "String.h"
#include "InputHandler.h"
#include "Types.h"
#include "Pieces.h"

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
	void executeCommand(const String& inputStr);
	void update();
	void start();
};