#include "GameState.h"

GameState::GameState() : playerTurn(Player::BLACK), gameOver(false) { board = new Board(); }

GameState::~GameState() { delete board; }

void GameState::printBoard() {
	std::cout << "  a b c d e f g h         h g f e d c b a" << std::endl;

	for (int i = 0; i < 8; ++i) {
		std::cout << 8 - i << " ";

		for (int j = 0; j < 8; ++j) {
			Square& square = board->operator[](7 - i)[j];
			if (square.getSpecialColor()) {
				std::cout << "\033[" << square.getSpecialColor() << "m";
			} else if ((i + j) % 2 == 0)
				std::cout << "\033[107m";
			else
				std::cout << "\033[101m";

			if (Piece* p = board->getPieceAtPos({7 - i, j}))
				std::cout << p->getEmoji() << " ";

			else
				std::cout << "  ";

			std::cout << "\033[0m";
		}

		std::cout << " " << 8 - i << "    " << i + 1 << " ";

		for (int j = 7; j >= 0; --j) {
			Square& square = board->operator[](i)[j];
			if (square.getAttackedBy(Player::BLACK))
				std::cout << "\033[108m";
			else if (square.getSpecialColor())
				std::cout << "\033[" << square.getSpecialColor() << "m";
			else if ((i + j) % 2 == 0)
				std::cout << "\033[107m";
			else
				std::cout << "\033[101m";

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

	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			(*board)[i][j].setSpecialColor(0);
		}
	}

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

		// std::cout << "Moving from " << fromPos.row << " " << fromPos.col << " to " << toPos.row << " " << toPos.col
		// << std::endl;

		if (board->movePiece(fromPos, toPos, playerTurn, error)) playerTurn = !playerTurn;
		// board->movePiece(fromPos, toPos, playerTurn, error);
	} else if (cmd == "mark") {
		String posStr;
		InputHandler::token(posStr, inputStr, p);
		Position pos = {InputHandler::charIntToBoardIndex(posStr[1]), InputHandler::charToBoardIndex(posStr[0])};

		Piece* piece = board->getPieceAtPos(pos);
		if (!piece) {
			error = "No piece at the selected position!";
			return;
		}

		board->calculateSquares();
		const Vector<Position>& squaresToMark = piece->getValidMoves();

		for (int i = 0; i < squaresToMark.size(); ++i) {
			board->operator[](squaresToMark[i].row)[squaresToMark[i].col].setSpecialColor(43);
		}
	}
}

bool GameState::hasGameEnded() {
	(*board).calculateSquares();

	stalemate = true;
	for (int i = 0; i < 8; ++i) {
		for (int j = 0; j < 8; ++j) {
			Piece* piece = (*board)[i][j].getPiece();
			if (!piece) continue;

			if (King* king = dynamic_cast<King*>(piece)) {
				if ((*board).getCheckExists() != -1 && king->getValidMoves().size() == 0) {
					checkmate = true;
				}
			} else {
				stalemate = false;
			}
		}
	}

	if (checkmate || stalemate) printBoard();

	if (checkmate) {
		std::cout << "Checkmate! " << !playerTurn << " wins!" << std::endl;
		return true;
	}

	if (stalemate) {
		std::cout << "Stalemate! The game is a draw!" << std::endl;
		return true;
	}

	return false;
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

	gameOver = hasGameEnded();
}

void GameState::start() {
	while (!gameOver) {
		update();
	}
}