#include "GameState.h"

#ifdef _WIN32
#include <windows.h>
void enableANSI() {
	HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
	DWORD dwMode = 0;
	GetConsoleMode(hOut, &dwMode);
	dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
	SetConsoleMode(hOut, dwMode);
	SetConsoleOutputCP(CP_UTF8);
	SetConsoleCP(CP_UTF8);
}
#endif

GameState::GameState() : playerTurn(Player::WHITE), gameOver(false) { board = new Board(); }

GameState::~GameState() { delete board; }

void GameState::drawSquare(Position pos) {
	if (int specialCol = (*board)[pos.row][pos.col].getSpecialColor()) {
		std::cout << "\033[" << specialCol << "m";
	} else {
		bool white = ((pos.row + pos.col) % 2 == 0);
		std::cout << (white ? "\033[107m" : "\033[101m");
	}

	if (Piece* piece = board->getPieceAtPos(pos))
		std::cout << piece->getEmoji() << ' ';
	else
		std::cout << "  ";

	std::cout << "\033[0m";
}

void GameState::printBoard() {
	// Clear
	std::cout << "\033[2J\033[H" << std::flush;

	std::cout << "  a b c d e f g h         h g f e d c b a\n";

	for (int i = 0; i < BOARD_SIZE; ++i) {
		int rowLeft = 7 - i;
		int rowRight = i;

		std::cout << (BOARD_SIZE - i) << ' ';

		for (int j = 0; j < BOARD_SIZE; ++j)
			drawSquare({rowLeft, j});

		std::cout << ' ' << (BOARD_SIZE - i) << "    " << (i + 1) << ' ';

		for (int j = 7; j >= 0; --j)
			drawSquare({rowRight, j});

		std::cout << ' ' << (i + 1) << "\n";
	}

	std::cout << "  a b c d e f g h         h g f e d c b a\n";
}

void GameState::checkForPawnPromotion() {
	int backrank = (!playerTurn == Player::WHITE) ? 7 : 0;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		Piece* piece = (*board)[backrank][i].getPiece();
		if (piece && dynamic_cast<Pawn*>(piece)) {
			if (piece->getColor() == !playerTurn) {
				String promotionChoice;
				while (true) {
					std::cout << "Pawn promotion available! Choose a piece (q, r, b, n): ";
					getline(std::cin, promotionChoice);
					if (promotionChoice == "q") {
						(*board).setPiece(new Queen(!playerTurn), {backrank, i});
						break;
					} else if (promotionChoice == "r") {
						(*board).setPiece(new Rook(!playerTurn), {backrank, i});
						break;
					} else if (promotionChoice == "b") {
						(*board).setPiece(new Bishop(!playerTurn), {backrank, i});
						break;
					} else if (promotionChoice == "n") {
						(*board).setPiece(new Knight(!playerTurn), {backrank, i});
						break;
					} else {
						std::cout << "Invalid choice!" << std::endl;
					}
				}
			}
		}
	}
}

void GameState::executeCommand(const String& inputStr) {
	String cmd;
	int p = 0;

	InputHandler::token(cmd, inputStr, p);

	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			(*board)[i][j].setSpecialColor(0);
		}
	}

	if (cmd == "mark") {
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
			(*board)[squaresToMark[i].row][squaresToMark[i].col].setSpecialColor(43);
		}
	} else if (cmd == "save") {
		String filename;
		InputHandler::token(filename, inputStr, p);

		String fullPath = "./savefiles/" + filename + ".save";
		std::ofstream outFile(fullPath.c_str(), std::ios::out);
		if (!outFile) {
			error = "Couldn't open file!";
			return;
		}
		outFile << board->serialize();
		outFile << (playerTurn == Player::WHITE ? 'w' : 'b');
		outFile.close();
	} else if (cmd == "load") {
		String filename;
		InputHandler::token(filename, inputStr, p);

		String fullPath = "./savefiles/" + filename + ".save";
		std::ifstream inFile(fullPath.c_str(), std::ios::in);
		if (!inFile) {
			error = "Couldn't open file!";
			return;
		}

		String boardData;
		char c;
		while (inFile.get(c)) {
			boardData.push_back(c);
		}
		inFile.close();

		constexpr size_t expectedSize = BOARD_SIZE * 9 + 1;
		if (boardData.size() != expectedSize) {
			error = "Invalid file format!";
			return;
		}

		playerTurn = (boardData[boardData.size() - 1] == 'w') ? Player::WHITE : Player::BLACK;

		lastSixMoves.clear();

		delete board;
		board = new Board(boardData);
	} else if (cmd == "quit") {
		quit = true;
	} else if (cmd == "help") {
		error = "Available commands: <from> <to>(move), mark <pos>, save <filename>, load <filename>, help, quit";
	} else {
		// Move
		String& from = cmd;
		String to;
		InputHandler::token(to, inputStr, p);
		Position fromPos = {InputHandler::charIntToBoardIndex(from[1]), InputHandler::charToBoardIndex(from[0])};
		Position toPos = {InputHandler::charIntToBoardIndex(to[1]), InputHandler::charToBoardIndex(to[0])};

		if (board->movePiece(fromPos, toPos, playerTurn, error)) playerTurn = !playerTurn;

		if (lastSixMoves.size() == 6) lastSixMoves.erase(0);
		lastSixMoves.push_back(from + to);

		checkForPawnPromotion();
	}
}

bool GameState::hasGameEnded() {
	bool onlyKings = true;
	for (int i = 0; i < BOARD_SIZE; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			Piece* piece = (*board)[i][j].getPiece();
			if (piece && dynamic_cast<King*>(piece) == nullptr) {
				onlyKings = false;
				break;
			}
		}
		if (!onlyKings) break;
	}
	if (onlyKings) {
		printBoard();
		std::cout << "Stalemate! The game is a draw!" << std::endl;
		return true;
	}

	bool inCheck = (board->getCheckExists() == static_cast<int>(playerTurn));
	bool hasLegalMoves = false;

	for (int i = 0; i < BOARD_SIZE && !hasLegalMoves; ++i) {
		for (int j = 0; j < BOARD_SIZE; ++j) {
			Piece* piece = (*board)[i][j].getPiece();
			if (piece && piece->getColor() == playerTurn && !piece->getValidMoves().empty()) {
				hasLegalMoves = true;
				break;
			}
		}
	}

	if (inCheck && !hasLegalMoves) {
		printBoard();
		std::cout << "Checkmate! " << !playerTurn << " wins!" << std::endl;
		return true;
	}

	bool repetition = false;
	if (lastSixMoves.size() == 6) {
		repetition = true;
		const String& W1 = lastSixMoves[0];
		const String& W2 = lastSixMoves[2];
		const String& W3 = lastSixMoves[4];
		bool whiteRep = (W1 == W3) && (W2 == Utility::invertMove(W1));

		const String& B1 = lastSixMoves[1];
		const String& B2 = lastSixMoves[3];
		const String& B3 = lastSixMoves[5];
		bool blackRep = (B1 == B3) && (B2 == Utility::invertMove(B1));

		repetition = whiteRep && blackRep;
	}

	if (repetition) {
		printBoard();
		std::cout << "Draw by repetition!" << std::endl;
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
		std::cout << "Enter command: ";
		getline(std::cin, inputStr);

	} while (!InputHandler::isExistingCommand(inputStr, error) || !InputHandler::areValidParams(inputStr, error));

	executeCommand(inputStr);

	(*board).calculateSquares();

	gameOver = hasGameEnded();
}

void GameState::start() {
#ifdef _WIN32
	enableANSI();
#endif

	while (!quit) {
		while (!gameOver && !quit) {
			update();
		}

		if (!quit) {
			std::cout << "Press Enter for new game...";
			std::cin.get();
			delete board;
			board = new Board();
			playerTurn = Player::WHITE;
			gameOver = false;
		}
	}
}