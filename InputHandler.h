#ifndef INPUTHANDLER_H
#define INPUTHANDLER_H

#include "String.h"
#include "Types.h"

class InputHandler {
  private:
	static bool isValidCoordinate(const String& coord); 

  public:
	static int charToBoardIndex(char c);

	static int charIntToBoardIndex(char c);

	static void token(String& dest, const String& str, int& p);

	static bool isExistingCommand(String& inputStr, String& error);

	static bool areValidParams(String& inputStr, String& error);
};

#endif