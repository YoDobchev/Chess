#include "GameState.h"
#include <ostream>

GameState::GameState() : playerTurn(Player::BLACK), gameOver(false) { board = new Board(); }

GameState::~GameState() { delete board; }

void GameState::printBoard() {
	// if (board->getPieceAtPos({6, 0})) {
	// 	std::cout << board->getPieceAtPos({6, 0})->getEmoji() << std::endl;
	// }
	std::cout << "  a b c d e f g h         h g f e d c b a" << std::endl;

	for (int i = 0; i < 8; ++i) {
		std::cout << 8 - i << " ";

		for (int j = 0; j < 8; ++j) {
			if ((i + j) % 2 == 0)
				std::cout << "\033[47m";
			else
				std::cout << "\033[41m";

			// std::cout << i<< " " << j << " ";
			if (Piece* p = board->getPieceAtPos({7 - i, j})) {
				std::cout << p->getEmoji() << " ";
				if (7 - i == 4 && j == 1) {
					if (p->getColor() == Player::WHITE) std::cout << "PNIESSS";
				}
			} else
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

void GameState::executeCommand(const String& inputStr) {
	String cmd;
	int p = 0;

	InputHandler::token(cmd, inputStr, p);

	if (cmd == "move") {
		String from, to;
		InputHandler::token(from, inputStr, p);
		InputHandler::token(to, inputStr, p);
		Position fromPos = {InputHandler::charIntToBoardIndex(from[1]), InputHandler::charToBoardIndex(from[0])};
		Position toPos = {InputHandler::charIntToBoardIndex(to[1]), InputHandler::charToBoardIndex(to[0])};

		// if (board->getPieceAtPos(fromPos)->getColor() != playerTurn) {
		// 	error = "It's not your turn";
		// 	return;
		// }

		if (board->movePiece(fromPos, toPos, playerTurn, error)) playerTurn = !playerTurn;
		// board->movePiece(fromPos, toPos, playerTurn, error);
	}
}

void GameState::update() {
	String inputStr;
	do {
		printBoard();
		if (error.size() > 0) {
			std::cout << error << std::endl;
			error.clear();
		}
		std::cout << (playerTurn == Player::WHITE ? "[WHITE] " : "[BLACK] ");
		std::cout << "enter command: ";
		getline(std::cin, inputStr);

	} while (!InputHandler::isExistingCommand(inputStr, error) || !InputHandler::areValidParams(inputStr, error));

	executeCommand(inputStr);
}

void GameState::start() {
	while (true) {
		update();
	}
}