#include <vector>
#include <fstream>
#include <set>
#include <iterator>
#include <iostream>

#include "GameField.h"
#include "Console.h"

namespace gf = gamefield;
namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace console;

gf::GameField::GameField() {
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;
	field.resize(height, std::vector<char>(width, DEAD_CELL));
	survive_rule = DEFAULT_SURVIVE_RULE;
	birth_rule = DEFAULT_BIRTH_RULE;
}

gf::GameField::GameField(std::string input_filename) {
	Console errout;
	std::ifstream inf(input_filename);
	int tmp = 0;
	if (!inf) {
		errout.writeError(CANNOT_OPEN_FILE);
		field.resize(height, std::vector<char>(width, DEAD_CELL));	//not good, need own map
		survive_rule = DEFAULT_SURVIVE_RULE;
		birth_rule = DEFAULT_BIRTH_RULE;
	}
	else {
		input_header(inf, *this);
		int current_pos = inf.tellg();
		calculate_size(inf, *this);
		inf.clear();
		inf.seekg(0, std::ios_base::beg);
		field.resize(height, std::vector<char>(width, DEAD_CELL));
		input_cells(inf, *this);
	}
	inf.close();
}

gf::GameField::~GameField() {

}


void gf::GameField::input_header(std::ifstream& inf, GameField& map) {
	Console errout;
	std::string strInput;
	int is_format = NOT_ENTERED, is_name = NOT_ENTERED, is_rule = NOT_ENTERED, line_index = 0, tmp = 0;
	for (; (line_index < NUM_OF_INIT_LINES) && getline(inf, strInput); line_index++) {
		if (strInput == FILE_FORMAT) {
			if (is_format == NOT_ENTERED) is_format = ENTERED;
			else {
				errout.writeError(MULTIPLE_FILE_FORMAT);
			}
		}
		else if (strInput.substr(0, SPEC_LENGTH) == UNIVERSE_NAME_SPEC) {
			if (is_name == NOT_ENTERED && strInput.length() > SPEC_LENGTH) {
				is_name = ENTERED;
				universe_name = strInput.substr(SPEC_LENGTH);
			}
			else {
				errout.writeError(MULTIPLE_UNIVERSE_NAME);
			}
		}
		else if (strInput.substr(0, SPEC_LENGTH) == RULES_SPEC) {
			if (is_rule == NOT_ENTERED && strInput.length() > SPEC_LENGTH) {
				tmp = input_rules(strInput, *this);
				if (tmp == CRITICAL_ERROR) {
					errout.writeError(WRONG_RULES_FORMAT);
					survive_rule = DEFAULT_SURVIVE_RULE;
					birth_rule = DEFAULT_BIRTH_RULE;
				}
				else if (tmp == NO_SURVIVE_RULE_ERROR) {
					errout.writeError(NO_SURVIVE_RULE);
					survive_rule = DEFAULT_SURVIVE_RULE;
				}
				is_rule = ENTERED;
			}
			else {
				errout.writeError(MULTIPLE_RULES);
			}
		}
	}
	if (is_format == NOT_ENTERED) {
		errout.writeError(NO_FILE_FORMAT);
	}
	if (is_name == NOT_ENTERED) {
		errout.writeError(NO_UNIVERSE_NAME);
		universe_name = DEFAULT_UNIVERSE_NAME;
	}
	if (is_rule == NOT_ENTERED) {
		errout.writeError(NO_GAME_RULES);
		survive_rule = DEFAULT_SURVIVE_RULE;
		birth_rule = DEFAULT_BIRTH_RULE;
	}
}

int gf::GameField::input_rules(std::string source, GameField &map) {
	int pos = CURRENT_POS;
	if (source[pos] != BIRTH_LETTER) {
		return CRITICAL_ERROR;
	}
	pos++;
	for (; pos < source.length(); pos++) {
		if (source[pos] >= LOWEST_RULE_NUMBER && source[pos] <= HIGHEST_RULE_NUMBER) {
			map.birth_rule.insert(source[pos] - CHAR_TO_NUM_COEF);
		}
		else if(source[pos] == SLASH) {
			pos++;
			break;
		}
	}
	if (pos >= source.length() || source[pos] != SURVIVE_LETTER) {
		return NO_SURVIVE_RULE_ERROR;
	}
	pos++;
	for (; pos < source.length(); pos++) {
		if (source[pos] >= LOWEST_RULE_NUMBER && source[pos] <= HIGHEST_RULE_NUMBER) {
			map.survive_rule.insert(source[pos] - CHAR_TO_NUM_COEF);
		}
	}
	return RULES_ENTERED;
}

void gf::GameField::calculate_size(std::ifstream& inf, GameField& map) {
	int x_cor = 0, y_cor = 0;
	std::string strInput;
	for (; getline(inf, strInput);) {
		if (sscanf(strInput.data(), "%d %d", &x_cor, &y_cor) == 2) {
			x_cor = abs(x_cor);
			y_cor = abs(y_cor);
			if (x_cor > map.width) {
				map.width = x_cor;
			}
			if (y_cor > map.height) {
				map.height = y_cor;
			}
		}
	}
	map.height = map.height + 1;
	map.width = map.width + 1;
}

void gf::GameField::input_cells(std::ifstream& inf, GameField& map) {
	int x_cor = 0, y_cor = 0;
	std::string strInput;
	for (; getline(inf, strInput);) {
		if (sscanf(strInput.data(), "%d %d", &x_cor, &y_cor) == 2) {
			if (x_cor < 0) x_cor = width + x_cor;
			if (y_cor < 0) y_cor = height + y_cor;
			map.field[y_cor][x_cor] = ALIVE_CELL;
		}
	}
}

gf::GameField& gf::GameField::operator=(const GameField& a) {
	width = a.width;
	height = a.height;
	survive_rule = a.survive_rule;
	birth_rule = a.birth_rule;
	universe_name = a.universe_name;
	field = a.field;
	return *this;
}

void gf::GameField::makeIteration(GameField& map) {
	static GameField tmp_map;
	tmp_map = map;
	for (int i = 1; i < map.height - 1; i++) {
		for (int j = 1; j < map.width - 1; j++) {
			update_center_cell(i, j, map, tmp_map);
		}
	}
	for (int i = 0; i < map.height; i++) {
		update_border_cell(i, 0, map, tmp_map);
		update_border_cell(i, map.width - 1, map, tmp_map);
	}
	for (int i = 0; i < map.width; i++) {
		update_border_cell(0, i, map, tmp_map);
		update_border_cell(map.height - 1, i, map, tmp_map);
	}
}

inline int gf::GameField::count_center_neighbours(int x, int y, GameField& map) {
	return (map.field[x + 1][y] + map.field[x - 1][y]
		+ map.field[x + 1][y + 1] + map.field[x - 1][y + 1]
		+ map.field[x + 1][y - 1] + map.field[x - 1][y - 1]
		+ map.field[x][y + 1] + map.field[x][y - 1]);
}

inline int gf::GameField::count_border_neighbours(int x, int y, GameField& map) {//there mistake
	return (map.field[(x + 1 + map.height) % map.height][(y + map.width) % map.width]
		+ map.field[(x - 1 + map.height) % map.height][(y + map.width) % map.width]
		+ map.field[(x + 1 + map.height) % map.height][(y + 1 + map.width) % map.width]
		+ map.field[(x - 1 + map.height) % map.height][(y + 1 + map.width) % map.width]
		+ map.field[(x + 1 + map.height) % map.height][(y - 1 + map.width) % map.width]
		+ map.field[(x - 1 + map.height) % map.height][(y - 1 + map.width) % map.width]
		+ map.field[(x + map.height) % map.height][(y + 1 + map.width) % map.width]
		+ map.field[(x + map.height) % map.height][(y - 1 + map.width) % map.width]);
}

inline void gf::GameField::update_center_cell(int x, int y, GameField& map, GameField& tmp_map) {
	if (tmp_map.field[x][y] == ALIVE_CELL && tmp_map.survive_rule.count(count_center_neighbours(x, y, tmp_map)) == 0) {
		map.field[x][y] = DEAD_CELL;
	}
	else if (tmp_map.field[x][y] == DEAD_CELL && tmp_map.birth_rule.count(count_center_neighbours(x, y, tmp_map)) != 0) {
		map.field[x][y] = ALIVE_CELL;
	}
}

inline void gf::GameField::update_border_cell(int x, int y, GameField& map, GameField& tmp_map) {
	if (tmp_map.field[x][y] == ALIVE_CELL && tmp_map.survive_rule.count(count_border_neighbours(x, y, tmp_map)) == 0) {
		map.field[x][y] = DEAD_CELL;
	}
	else if (tmp_map.field[x][y] == DEAD_CELL && tmp_map.birth_rule.count(count_border_neighbours(x, y, tmp_map)) > 0) {
		map.field[x][y] = ALIVE_CELL;
	}
}

void gf::GameField::iterate(int count, int silence) {
	Console show_map;
	iterations += count;
	std::cout << universe_name << '\n' << RULES_SPEC << BIRTH_LETTER;
	for (std::set<int>::iterator birth_num = birth_rule.begin(); birth_num != birth_rule.end(); ++birth_num) {
		std::cout << (char)(*birth_num + CHAR_TO_NUM_COEF);
	}
	std::cout << SLASH << SURVIVE_LETTER;
	for (std::set<int>::iterator survive_num = survive_rule.begin(); survive_num != survive_rule.end(); ++survive_num) {
		std::cout << (char)(*survive_num + CHAR_TO_NUM_COEF);
	}
	std::cout << '\n' << iterations << '\n';
	for (int i = 0; i < count; i++) {
		makeIteration(*this);
	}
	if (silence == NO_SILENCE) {
		show_map.printMap(return_map(), height, width);
	}
}

std::vector<std::vector<char>> gf::GameField::return_map() {
	std::vector<std::vector<char>> map_copy = field;
	return map_copy;
}

void gf::GameField::dump(std::string output_file) {
	std::ofstream fout;
	fout.open(input_file);
	fout << FILE_FORMAT << '\n' << UNIVERSE_NAME_SPEC << universe_name << '\n' << RULES_SPEC << BIRTH_LETTER;
	for (std::set<int>::iterator birth_num = birth_rule.begin(); birth_num != birth_rule.end(); ++birth_num) {
		fout << (char)(*birth_num + CHAR_TO_NUM_COEF);
	}
	fout << SLASH << SURVIVE_LETTER;
	for (std::set<int>::iterator survive_num = survive_rule.begin(); survive_num != survive_rule.end(); ++survive_num) {
		fout << (char)(*survive_num + CHAR_TO_NUM_COEF);
	}
	fout << '\n';
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (field[i][j] == ALIVE_CELL) {
				fout << i << " " << j << '\n';
			}
		}
	}
	fout.close();
}

void gf::GameField::run() {
	if (mode == DEFAULT_MODE) {
		iterate(iterations, SILENCE);
		(*this).dump(output_file);
	}
	Console coutput;
	std::string argument;
	int returned_code = 0, ticks = 0, end_of_programm = 0;
	for (;end_of_programm != END_OF_PROGRAMM;) {
		returned_code = coutput.read_command(argument);
		switch (returned_code) {
		case DUMP:
			if (argument.length() == 0) {
				coutput.writeError(NO_FILENAME);
				break;
			}
			(*this).dump(argument);
			coutput.writeMessage(SUCCESFUL_DUMP);
			break;
		case HELP:
			coutput.writeMessage(HELP_MESSAGE);
			break;
		case EXIT:
			coutput.writeMessage(EXIT_MESSAGE);
			end_of_programm = END_OF_PROGRAMM;
			break;
		case TICK:
			if (argument.length() == 0) {
				coutput.writeError(NO_TICKS);
				break;
			}
			ticks = std::stoi(argument);
			if (ticks <= 0) {
				coutput.writeError(WRONG_TICKS);
				break;
			}
			iterate(ticks);
			break;
		default:
			coutput.writeError(WRONG_OPTION);
		}
	}
}

gf::ArgsContainer::ArgsContainer() {

}

gf::ArgsContainer::~ArgsContainer() {

}

gf::ArgsContainer::ArgsContainer(int argc, char** argv) {
	po::options_description desc("Allowed options:");
	desc.add_options()
		("help", "Writes help message")
		("iterations,i", po::value<int>(&buf_field.iterations)->default_value(DEFAULT_ITERATIONS), "Number of universe iterations")
		("input", po::value<std::string>(&buf_field.input_file)->default_value(DEFAULT_INPUT_FILENAME), "Name of file for input")
		("output,o", po::value<std::string>(&buf_field.output_file)->default_value(DEFAULT_OUTPUT_FILENAME), "Name of file for output")
		;

	po::variables_map var_map;
	po::store(po::parse_command_line(argc, argv, desc), var_map);
	po::notify(var_map);
	if (var_map.count("help")) {
		std::cout << desc << '\n';
	}

	po::positional_options_description desc_pos;
	desc_pos.add("input", -1);

	po::variables_map pos_map;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(desc_pos).run(), pos_map);
	po::notify(pos_map);
	if (pos_map.count("input")) {
		buf_field.input_file = pos_map["input"].as<std::string>();
	}
	else {
		buf_field.input_file = var_map["input"].as<std::string>();
	}
	buf_field.output_file = var_map["output"].as<std::string>();
	buf_field.iterations = var_map["iterations"].as<int>();
}

void gf::ArgsContainer::gameFieldInitialization(gamefield::GameField& map) {
	gamefield::GameField tmp(buf_field.input_file);
	map = tmp;
	map.iterations = buf_field.iterations;
	map.input_file = buf_field.input_file;
	map.output_file = buf_field.output_file;
	if (buf_field.output_file != DEFAULT_OUTPUT_FILENAME && buf_field.input_file != DEFAULT_INPUT_FILENAME && buf_field.iterations != DEFAULT_ITERATIONS) {
		tmp.mode = OFFLINE_MODE;
	} else if (buf_field.input_file != DEFAULT_INPUT_FILENAME) {
		tmp.mode = ONLINE_MODE;
	}
	else {
		tmp.mode = DEFAULT_MODE;
	}
}
