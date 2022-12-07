#pragma once
#include <string>
#include <iostream>
#include <vector>

namespace console {
	enum errorsCodes {
		MULTIPLE_ITERATIONS = 1,
		INCORRECT_ITERATIONS = 2,
		INCORRECT_INPUT_FILENAME = 3,
		INCORRECT_ITERATIONS_LONG = 4,
		INCORRECT_INPUT_FILENAME_LONG = 5,
		INCORRECT_PARAMETER = 6,
		NONINITIALIZED_ITERATIONS = 7,
		NONINITIALIZED_INPUT_FILENAME = 8,
		CANNOT_OPEN_FILE = 9,
		WRONG_FILE_FORMAT = 10,
		MULTIPLE_FILE_FORMAT = 11,
		WRONG_UNIVERSE_NAME = 12,
		MULTIPLE_UNIVERSE_NAME = 13,
		WRONG_RULES_FORMAT = 14,
		WRONG_BIRTH_RULE = 15,
		WRONG_SURVIVE_RULE = 16,
		MULTIPLE_RULES = 17,
		NO_UNIVERSE_NAME = 18,
		NO_GAME_RULES = 19,
		NO_FILE_FORMAT = 20,
		WRONG_BIRTH_RULE_FORMAT = 21,
		NO_SURVIVE_RULE = 22,
		WRONG_SYMBOLS = 23,
		MULTIPLE_BIRTH_RULE = 24,
		MULTIPLE_SURVIVAL_RULE = 25,
		WRONG_SURVIVAL_RULE_FORMAT = 26,
		MULTIPLE_ALIVE_CELL = 27,
		WRONG_RULE_FORMAT_DEFAULT = 28,
		NO_FILENAME = 29,
		NO_TICKS = 30,
		WRONG_TICKS = 31,
		WRONG_OPTION = 32
	};
	enum commandCodes {
		WRONG_COMMAND = 1,
		DUMP = 2,
		TICK = 3,
		EXIT = 4,
		HELP = 5
	};
	const int COMMAND_LENGTH = 5;

	const std::string DUMP_STR = "dump ";
	const std::string HELP_STR = "help";
	const std::string EXIT_STR = "exit";
	const std::string TICK_STR = "tick";
	const std::string T_STR = "t";
	const std::string LEFTARROW = "<";
	const std::string RIGHTARROW = ">";

	class Console {
	public:
		Console();
		~Console();
		void writeError(int errorId);
		void writeMessage(std::string message);
		int read_command(std::string& argument);
		void printMap(std::vector<std::vector<char>> field, int height, int width);
	};
}