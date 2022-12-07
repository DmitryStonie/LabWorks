#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>

namespace console {
	//command codes
	const int WRONG_COMMAND = 1;
	const int DUMP = 2;
	const int TICK = 3;
	const int EXIT = 4;
	const int HELP = 5;
	const int COMMAND_LENGTH = 5;
	const int ALIVE_CELL = 1;
	//error codes
	const int CANNOT_OPEN_FILE = 9;
	const int MULTIPLE_FILE_FORMAT = 11;
	const int WRONG_UNIVERSE_NAME = 12;
	const int MULTIPLE_UNIVERSE_NAME = 13;
	const int WRONG_RULES_FORMAT = 14;
	const int NO_SURVIVE_RULE = 22;
	const int MULTIPLE_RULES = 17;
	const int NO_FILE_FORMAT = 20;
	const int NO_UNIVERSE_NAME = 18;
	const int NO_GAME_RULES = 19;
	const int NO_FILENAME = 29;
	const int NO_TICKS = 30;
	const int WRONG_TICKS = 31;
	const int WRONG_OPTION = 32;
	//command constants
	const std::string DUMP_STR = "dump ";
	const std::string HELP_STR = "help";
	const std::string EXIT_STR = "exit";
	const std::string TICK_STR = "tick ";
	const std::string T_STR = "t";
	const std::string LEFTARROW = "<";
	const std::string RIGHTARROW = ">";
	
	const char ALIVE_CELL_SYMB = 'O';
	const char DEAD_CELL_SYMB = '#';

	class Console {
		std::map<int, const std::string> errors = {
			{CANNOT_OPEN_FILE, "Error: can't open file."},
			{MULTIPLE_FILE_FORMAT, "Error: multiple file format. "},
			{WRONG_UNIVERSE_NAME, "Error: wrong universe name."},
			{MULTIPLE_UNIVERSE_NAME, "Error: multiple universe name. Used first name."},
			{WRONG_RULES_FORMAT, "Error: wrong rules format."},
			{NO_SURVIVE_RULE,"Error: no survive rule. Used default rule."},
			{MULTIPLE_RULES,"Error: multiple rules. Used first rule."},
			{NO_FILE_FORMAT,"Error: no file format. First string must be #Life 1.06."},
			{NO_UNIVERSE_NAME,"Error: no universe name. Used default name."},
			{NO_GAME_RULES,"Error: no game rules. Used default rules."},
			{NO_FILENAME, "Error: no file for dump."},
			{NO_TICKS, "Error: no number of ticks."},
			{WRONG_TICKS, "Error: ticks can't be <= 0."},
			{WRONG_OPTION, "Error: wrong command. Use \"help\" for display all commands."}
		};
	public:
		Console();
		~Console();
		void writeError(int errorId);
		void writeMessage(std::string message);
		int read_command(std::string& argument);
		void printMap(std::vector<std::vector<char>> field, int height, int width);
	};
}