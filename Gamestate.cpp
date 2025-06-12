#include "GameState.h"

// void GameState::
void GameState::startGame() {
	String inputStr, error;
	do {
		// printBoard();
		if (error.size() > 0) {
			std::cout << error << std::endl;
			error.clear();
		}
		std::cout << "enter command: ";
		getline(std::cin, inputStr);
	} while (!InputHandler::isExistingCommand(inputStr, error) || !InputHandler::areValidParams(inputStr, error));
}