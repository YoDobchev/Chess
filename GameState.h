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
	bool checkmate, stalemate;
  public:
	GameState();
	~GameState();
	void printBoard();
	void executeCommand(const String& inputStr);
	bool hasGameEnded();
	void update();
	void start();
};