#include <iostream>
// #include "board.h"
#include "String.h"
#include "EventHandler.h"

class GameState {
  private:
	bool playerTurn;
	// Board board;
  public:
	// void printBoard(const Board& board) {}

	void startGame() {
		String inputStr, error;
		do {
			// printBoard();
			if (error.size() > 0) {
				std::cout << error << std::endl;
				error.clear();
			}
			std::cout << "enter command: ";
			getline(std::cin, inputStr);
		} while (!EventHandler::isExistingCommand(inputStr, error) || !EventHandler::areValidParams(inputStr, error));

		// EventHandler::executeCommand();
	}
};