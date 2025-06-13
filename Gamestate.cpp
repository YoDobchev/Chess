#include "GameState.h"

GameState::GameState() : playerTurn(Player::WHITE), gameOver(false) { board = new Board(); }

GameState::~GameState() { delete board; }

void GameState::printBoard() {
	std::cout << "  a b c d e f g h         h g f e d c b a\n";

	for (int i = 0; i < 8; ++i) {
		std::cout << 8 - i << " ";

		for (int j = 0; j < 8; ++j) {
			if ((i + j) % 2 == 0)
				std::cout << "\033[47m";
			else
				std::cout << "\033[41m";

			if (Piece* p = board->getPieceAtPos({7 - i, j}))
				std::cout << p->getEmoji() << " ";
			else
				std::cout << "  ";

			std::cout << "\033[0m";
		}

		std::cout << " " << 8 - i << "    " << i + 1 << " ";

		for (int j = 7; j >= 0; --j) {
			if ((i + j) % 2 == 0)
				std::cout << "\033[47m";
			else
				std::cout << "\033[41m";

			if (Piece* p = board->getPieceAtPos({i, j}))
				std::cout << p->getEmoji() << " ";
			else
				std::cout << "  ";

			std::cout << "\033[0m";
		}

		std::cout << " " << i + 1 << "\n";
	}

	std::cout << "  a b c d e f g h         h g f e d c b a\n";
}

void GameState::update() {
	String inputStr;
	do {
		printBoard();
		if (error.size() > 0) {
			std::cout << error << std::endl;
			error.clear();
		}
		std::cout << "enter command: ";
		getline(std::cin, inputStr);
	} while (!InputHandler::isExistingCommand(inputStr, error) || !InputHandler::areValidParams(inputStr, error));
}

void GameState::start() { update(); }