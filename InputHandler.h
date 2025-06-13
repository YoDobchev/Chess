#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "String.h"

class InputHandler {
  private:
	static bool isValidCoordinate(const String& coord) {
		if (coord.size() != 2) return false;
		char col = coord[0];
		char row = coord[1];
		return (col >= 'a' && col <= 'h') && (row >= '1' && row <= '8');
	}

  public:
	static int charToBoardIndex(char c) {
		if (c >= 'a' && c <= 'h') return c - 'a';
		if (c >= 'A' && c <= 'H') return c - 'A';
		return 0;
	}

	static int charIntToBoardIndex(char c) {
		if (c >= '1' && c <= '8') return c - '1';
		return 0;
	}

	static void token(String& dest, const String& str, int& p) {
		dest.clear();
		while (str[p] == ' ')
			p++;

		while (str[p] && str[p] != ' ')
			dest.push_back(str[p++]);
	}

	static bool isExistingCommand(String& inputStr, String& error) {
		const String validCmds[] = {"move", "mark", "save", "load"};
		String cmd;
		int p = 0;

		token(cmd, inputStr, p);

		for (int i = 0; i < sizeof(validCmds) / sizeof(validCmds[0]); ++i) {
			if (cmd == validCmds[i]) return true;
		}

		error = "Command not found!";

		return false;
	}

	static bool areValidParams(String& inputStr, String& error) {
		int p = 0;
		String cmd;
		token(cmd, inputStr, p);

		if (cmd == "move") {
			String from, to;
			token(from, inputStr, p);
			token(to, inputStr, p);

			if (from.empty() || to.empty()) {
				error = "Missing parameters for move command!";
				return false;
			}
			if (!isValidCoordinate(from) || !isValidCoordinate(to)) {
				error = "Invalid move coordinates!";
				return false;
			}
			if (from[0] == to[0] && from[1] == to[1]) {
				error = "Move cannot be the same square!";
				return false;
			}
		} else if (cmd == "mark") {
			String pos;
			token(pos, inputStr, p);
			if (pos.empty()) {
				error = "Position cannot be empty!";
				return false;
			}

			if (!isValidCoordinate(pos)) {
				error = "Invalid position coordinates!";
				return false;
			}
		} else if (cmd == "save" || cmd == "load") {
			String filename;
			token(filename, inputStr, p);
			if (filename.empty()) {
				error = "Filename cannot be empty!";
				return false;
			}
		}

		return true;
	}

	static void executeCommand(const char* str, ...) {
		int p = 0;
		char cmd[64];
	}
};

#endif