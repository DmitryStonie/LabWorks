#pragma once
#include <vector>
#include <string>
#include <set> 
#include <string>
#include <iostream>
#include <map>
#include <boost/program_options.hpp>
#include "../GameTest/pch.h"

namespace po = boost::program_options;

namespace gamefield {
	//default values
	const std::set<int> DEFAULT_SURVIVE_RULE{ 2, 3 };
	const std::set<int> DEFAULT_BIRTH_RULE{ 3 };
	const std::string DEFAULT_UNIVERSE_NAME = ("DefaultUniverseName");
	const int DEFAULT_WIDTH = 15;
	const int DEFAULT_HEIGHT = 15;
	//cells
	const int ALIVE_CELL = 1;
	const int DEAD_CELL = 0;
	
	//for file parser
	const int NON_CRITICAL_ERROR = 2;
	const int CRITICAL_ERROR = 3;
	const int NO_SURVIVE_RULE_ERROR = 4;
	const int RULES_ENTERED = 5;
	static const std::string FILE_FORMAT = "#Life 1.06";
	static const std::string UNIVERSE_NAME_SPEC = "#N ";
	static const std::string RULES_SPEC = "#R ";
	const char SLASH = '/';
	const char BIRTH_LETTER = 'B';
	const char SURVIVE_LETTER = 'S';
	
	const int SPEC_LENGTH = 3;
	const char CHAR_TO_NUM_COEF = 48;
	const int END_OF_PROGRAMM = 1;
	
	//commands messages
	const std::string SUCCESFUL_DUMP_MESSAGE = "Dump completed!\n";
	const std::string HELP_MESSAGE = "This is a help message\n";
	const std::string EXIT_MESSAGE = "End of the program...\n";
	
	//iterate mode
	const int SILENCE = 1;
	const int NO_SILENCE = 0;
	
	//gamemodes
	const int ONLINE_MODE = 1;
	const int DEFAULT_MODE = 2;
	const int OFFLINE_MODE = 3;
	
	class GameField {
		friend class GameFieldTest;
		FRIEND_TEST(TestSuite, poswidth);
		FRIEND_TEST(TestSuite, negwidth);
		FRIEND_TEST(TestSuite, toroidal);
		FRIEND_TEST(TestSuite, update_cells);
		FRIEND_TEST(TestSuite, dump_file);
		FRIEND_TEST(TestSuite, long_run);
		FRIEND_TEST(TestSuite, open_files);
		FRIEND_TEST(TestSuite, multiple_data);

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

		friend class ArgsContainer;
	public:
		GameField();
		~GameField();
		GameField(std::string input_filename);
		GameField& operator=(const GameField& a);
		void makeIteration(GameField& map);
		void iterate(int count, int silence = NO_SILENCE);
		std::vector<std::vector<char>> return_map();
		void dump(std::string output_file);
		void run();
	};

	const int DEFAULT_ITERATIONS = 0;
	const std::string DEFAULT_OUTPUT_FILENAME = "defaultOutput.txt";
	const std::string DEFAULT_INPUT_FILENAME = "defaultUniverse.txt";

	class ArgsContainer {
		GameField buf_field;
		po::options_description desc;
		po::positional_options_description pos_desc;
		po::variables_map var_map;
		po::variables_map pos_map;
	public:
		ArgsContainer();
		~ArgsContainer();
		ArgsContainer(int argc, char** argv);
		void gameFieldInitialization(GameField& map);
	};
}