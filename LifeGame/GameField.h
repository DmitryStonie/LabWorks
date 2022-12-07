#pragma once
#include <vector>
#include <string>
#include <set> 

namespace gamefield {
	const std::set<int> DEFAULT_SURVIVE_RULE{ 2, 3 };
	const std::set<int> DEFAULT_BIRTH_RULE{ 3 };
	const int DEFAULT_WIDTH = 72;
	const int DEFAULT_HEIGHT = 48;
	const int NON_CRITICAL_ERROR = 2;
	const int CRITICAL_ERROR = 3;
	const int RULES_ENTERED = 4;
	const int ALIVE_CELL = 1;
	const int DEAD_CELL = 0;
	const std::string DEFAULT_UNIVERSE_NAME = ("DefaultUniverseName");
	const std::string FILE_FORMAT = "#Life 1.06";
	const std::string UNIVERSE_NAME_SPEC = "#N ";
	const std::string RULES_SPEC = "#R ";
	const int NUM_OF_INIT_LINES = 3;
	const int CURRENT_POS = 3;
	const int SPEC_LENGTH = 3;
	const char BIRTH_LETTER = 'B';
	const char SURVIVE_LETTER = 'S';
	const char LOWEST_RULE_NUMBER = '0';
	const char HIGHEST_RULE_NUMBER = '8';
	const char CHAR_TO_NUM_COEF = 48;
	const int NOT_ENTERED = 0;
	const int ENTERED = 1;
	const int NO_SURVIVE_RULE_ERROR = 2;
	const char SLASH = '/';

	enum commandCodes {
		WRONG_COMMAND = 1,
		DUMP = 2,
		TICK = 3,
		EXIT = 4,
		HELP = 5
	};

	class GameField {
		std::vector<std::vector<char>> field;
		std::string universe_name;
		std::set<int> survive_rule;
		std::set<int> birth_rule;
		std::string input_file;
		std::string output_file;
		int width;
		int height;
		int iterations;
		int mode;
		int input_rules(std::string source, GameField& map);
		void input_header(std::ifstream& inf, GameField& map);
		void input_cells(std::ifstream& inf, GameField& map);
		void calculate_size(std::ifstream& inf, GameField& map);
		inline int count_center_neighbours(int x, int y, GameField& map);
		inline int count_border_neighbours(int x, int y, GameField& map);
		inline void update_center_cell(int x, int y, GameField& map, GameField& tmp_map);
		inline void update_border_cell(int x, int y, GameField& map, GameField& tmp_map);
	public:
		GameField();
		GameField(std::string input_filename);
		GameField& operator=(const GameField& a);
		~GameField();
		void makeIteration(GameField& map);
		void iterate(int& current_iteration, int count);
		std::vector<std::vector<char>> return_map();
		void dump(std::string output_file);
		void run();
	};
}