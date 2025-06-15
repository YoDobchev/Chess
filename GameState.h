#include <iostream>
#include <fstream>
#include "Board.h"
#include "String.h"
#include "InputHandler.h"
#include "Types.h"
#include "Pieces.h"

class GameState {
  private:
	bool gameOver, quit;
	Player playerTurn;
	Board* board;
	String error;
	Vector<String> lastSixMoves;

  public:
	GameState();
	~GameState();
	void drawSquare(Position pos);
	void printBoard();
	void checkForPawnPromotion();
	void executeCommand(const String& inputStr);
	bool hasGameEnded();
	void update();
	void start();
};