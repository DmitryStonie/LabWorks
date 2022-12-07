#include "Console.h"
#include <fstream>

console::Console::Console(){

}

console::Console::~Console() {

}

void console::Console::writeError(int errorId) {
	std::cerr << errors[errorId] << std::endl;
}

void console::Console::printMap(std::vector<std::vector<char>> field, int height, int width) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (field[i][j] == ALIVE_CELL) {
				std::cout << ALIVE_CELL_SYMB << ' ';
			}
			else {
				std::cout << DEAD_CELL_SYMB << ' ';
			}
		}
		std::cout << '\n';
	}
}

int console::Console::read_command(std::string &argument) {
	std::string input, tmp;
	getline(std::cin, input);
	tmp = input.substr(0, COMMAND_LENGTH);
	if (tmp == DUMP_STR) {
		if (input.find(LEFTARROW, 0) == std::string::npos || input.find(RIGHTARROW, 0) == std::string::npos) return WRONG_COMMAND;
		else argument = input.substr(input.find(LEFTARROW, 0) + 1, input.find(RIGHTARROW, 0) - input.find(LEFTARROW, 0) - 1);
		return DUMP;
	}
	else if(tmp == HELP_STR) {
		return HELP;
	}
	else if(tmp == EXIT_STR) {
		return EXIT;
	}
	else if (tmp == TICK_STR || tmp == T_STR) {
		if (input.find(LEFTARROW, 0) == std::string::npos || input.find(RIGHTARROW, 0) == std::string::npos) return WRONG_COMMAND;
		else argument = input.substr(input.find(LEFTARROW, 0) + 1, input.find(RIGHTARROW, 0) - input.find(LEFTARROW, 0) - 1);
		return TICK;
	}
	else {
		return WRONG_COMMAND;
	}
}

void console::Console::writeMessage(std::string message) {
	std::cout << message;
}
