#include <vector>
#include "GameField.h"
#include "Console.h"
#include <fstream>
#include <set>
#include <iterator>

using namespace std;

const set<int> DEFAULT_SURVIVE_RULE{ 2, 3 };
const set<int> DEFAULT_BIRTH_RULE{ 3 };
const int DEFAULT_WIDTH = 72;
const int DEFAULT_HEIGHT = 48;
const int NON_CRITICAL_ERROR = 2;
const int CRITICAL_ERROR = 3;
const int RULES_ENTERED = 4;
const int ALIVE_CELL = 1;
const int DEAD_CELL = 0;
const string DEFAULT_UNIVERSE_NAME = ("DefaultUniverseName");
const string FILE_FORMAT = "#Life 1.06";
const string UNIVERSE_NAME_SPEC = "#N ";
const string RULES_SPEC = "#R ";
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


GameField::GameField() {
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;
	field.resize(height, vector<char>(width, DEAD_CELL));
	survive_rule = DEFAULT_SURVIVE_RULE;
	birth_rule = DEFAULT_BIRTH_RULE;
}

GameField::GameField(string input_filename) {
	ErrorOutput errout;
	ifstream inf(input_filename);
	int tmp = 0;
	if (!inf) {
		errout.writeToConsole(CANNOT_OPEN_FILE);
		field.resize(height, vector<char>(width, DEAD_CELL));	//not good, need own map
		survive_rule = DEFAULT_SURVIVE_RULE;
		birth_rule = DEFAULT_BIRTH_RULE;
	}
	else {
		input_header(inf, *this);
		int current_pos = inf.tellg();
		calculate_size(inf, *this);
		inf.clear();
		inf.seekg(0, ios_base::beg);
		field.resize(height, vector<char>(width, DEAD_CELL));
		input_cells(inf, *this);
	}
	inf.close();
}

GameField::~GameField() {

}

void GameField::input_header(ifstream& inf, GameField& map) {
	ErrorOutput errout;
	string strInput;
	int is_format = NOT_ENTERED, is_name = NOT_ENTERED, is_rule = NOT_ENTERED, line_index = 0, tmp = 0;
	for (; (line_index < NUM_OF_INIT_LINES) && getline(inf, strInput); line_index++) {
		if (strInput == FILE_FORMAT) {
			if (is_format == NOT_ENTERED) is_format = ENTERED;
			else {
				errout.writeToConsole(MULTIPLE_FILE_FORMAT);
			}
		}
		else if (strInput.substr(0, SPEC_LENGTH) == UNIVERSE_NAME_SPEC) {
			if (is_name == NOT_ENTERED && strInput.length() > SPEC_LENGTH) {
				is_name = ENTERED;
				universe_name = strInput.substr(SPEC_LENGTH);
			}
			else {
				errout.writeToConsole(MULTIPLE_UNIVERSE_NAME);
			}
		}
		else if (strInput.substr(0, SPEC_LENGTH) == RULES_SPEC) {
			if (is_rule == NOT_ENTERED && strInput.length() > SPEC_LENGTH) {
				tmp = input_rules(strInput, *this);
				if (tmp == CRITICAL_ERROR) {
					errout.writeToConsole(WRONG_RULES_FORMAT);
					survive_rule = DEFAULT_SURVIVE_RULE;
					birth_rule = DEFAULT_BIRTH_RULE;
				}
				else if (tmp == NO_SURVIVE_RULE_ERROR) {
					errout.writeToConsole(NO_SURVIVE_RULE);
					survive_rule = DEFAULT_SURVIVE_RULE;
				}
				is_rule = ENTERED;
			}
			else {
				errout.writeToConsole(MULTIPLE_RULES);
			}
		}
	}
	if (is_format == NOT_ENTERED) {
		errout.writeToConsole(NO_FILE_FORMAT);
	}
	if (is_name == NOT_ENTERED) {
		errout.writeToConsole(NO_UNIVERSE_NAME);
		universe_name = DEFAULT_UNIVERSE_NAME;
	}
	if (is_rule == NOT_ENTERED) {
		errout.writeToConsole(NO_GAME_RULES);
		survive_rule = DEFAULT_SURVIVE_RULE;
		birth_rule = DEFAULT_BIRTH_RULE;
	}
}

int GameField::input_rules(string source, GameField &map) {
	ErrorOutput errout;
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

void GameField::calculate_size(ifstream& inf, GameField& map) {
	int x_cor = 0, y_cor = 0;
	string strInput;
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

void GameField::input_cells(ifstream& inf, GameField& map) {
	int x_cor = 0, y_cor = 0;
	string strInput;
	for (; getline(inf, strInput);) {
		if (sscanf(strInput.data(), "%d %d", &x_cor, &y_cor) == 2) {
			if (x_cor < 0) x_cor = width + x_cor;
			if (y_cor < 0) y_cor = height + y_cor;
			map.field[y_cor][x_cor] = ALIVE_CELL;
		}
	}
}

GameField& GameField::operator=(const GameField& a) {
	width = a.width;
	height = a.height;
	survive_rule = a.survive_rule;
	birth_rule = a.birth_rule;
	universe_name = a.universe_name;
	field = a.field;
	return *this;
}

void GameField::makeIteration(GameField& map) {
	static GameField tmp_map;
	tmp_map = map;
	int cell = 0;
	for (int i = 1; i < map.height - 1; i++) {
		for (int j = 1; j < map.width - 1; j++) {
			cell = tmp_map.field[i][j];
			update_center_cell(i, j, tmp_map, map);
		}
	}
	for (int i = 0; i < map.height; i++) {
		update_border_cell(i, 0, map, tmp_map);
		update_border_cell(i, map.width - 1, map, tmp_map);
	}
	for (int i = 0; i < map.height; i++) {
		update_border_cell(0, i, map, tmp_map);
		update_border_cell(map.height - 1, i, map, tmp_map);
	}
}

inline int GameField::count_center_neighbours(int x, int y, GameField& map) {
	return (map.field[x + 1][y] + map.field[x - 1][y]
		+ map.field[x + 1][y + 1] + map.field[x - 1][y + 1]
		+ map.field[x + 1][y - 1] + map.field[x - 1][y - 1]
		+ map.field[x][y + 1] + map.field[x][y - 1]);
}

inline int GameField::count_border_neighbours(int x, int y, GameField& map) {
	return (map.field[(x + 1 + map.width)%map.width][(y + map.height) % map.height]
		+ map.field[(x - 1 + map.width) % map.width][(y + map.height) % map.height]
		+ map.field[(x + 1 + map.width) % map.width][(y + 1 + map.height) % map.height]
		+ map.field[(x - 1 + map.width) % map.width][(y + 1 + map.height) % map.height]
		+ map.field[(x + 1 + map.width) % map.width][(y - 1 + map.height) % map.height]
		+ map.field[(x - 1 + map.width) % map.width][(y - 1 + map.height) % map.height]
		+ map.field[(x + map.width) % map.width][(y + 1 + map.height) % map.height]
		+ map.field[(x + map.width) % map.width][(y - 1 + map.height) % map.height]);
}

inline void GameField::update_center_cell(int x, int y, GameField& map, GameField& tmp_map) {
	if (tmp_map.field[x][y] == ALIVE_CELL && tmp_map.survive_rule.count(count_center_neighbours(x, y, tmp_map)) == 0) {
		map.field[x][y] = DEAD_CELL;
	}
	else if (tmp_map.field[x][y] == DEAD_CELL && tmp_map.birth_rule.count(count_center_neighbours(x, y, tmp_map)) > 0) {
		map.field[x][y] = ALIVE_CELL;
	}
}
inline void GameField::update_border_cell(int x, int y, GameField& map, GameField& tmp_map) {
	if (tmp_map.field[x][y] == ALIVE_CELL && tmp_map.survive_rule.count(count_border_neighbours(x, y, tmp_map)) == 0) {
		map.field[x][y] = DEAD_CELL;
	}
	else if (tmp_map.field[x][y] == DEAD_CELL && tmp_map.birth_rule.count(count_border_neighbours(x, y, tmp_map)) > 0) {
		map.field[x][y] = ALIVE_CELL;
	}
}

void GameField::printMap() {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (field[i][j] == ALIVE_CELL) {
				cout << 'O';
			} else {
				cout << '#';
			}
		}
		cout << '\n';
	}
}

void GameField::dump(string filename) {
	ofstream fout(filename);
	fout << FILE_FORMAT << '\n' << universe_name << '\n' << RULES_SPEC << BIRTH_LETTER;
	for (set<int>::iterator birth_num = birth_rule.begin(); birth_num != birth_rule.end(); ++birth_num) {
		fout << (char)(*birth_num + CHAR_TO_NUM_COEF);
	}
	fout << SLASH << SURVIVE_LETTER;
	for (set<int>::iterator survive_num = survive_rule.begin(); survive_num != survive_rule.end(); ++survive_num) {
		fout << (char)(*survive_num + CHAR_TO_NUM_COEF);
	}
	fout << '\n';
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (field[i][j] == ALIVE_CELL) {
				fout << 'O';
			}
			else {
				fout << '#';
			}
		}
		fout << '\n';
	}
	fout.close();
}

void GameField::iterate(int current_iteration, int count) {
	current_iteration += count;
	cout << universe_name << '\n' << RULES_SPEC << BIRTH_LETTER;
	for (set<int>::iterator birth_num = birth_rule.begin(); birth_num != birth_rule.end(); ++birth_num) {
		cout << (char)(*birth_num + CHAR_TO_NUM_COEF);
	}
	cout << SLASH << SURVIVE_LETTER;
	for (set<int>::iterator survive_num = survive_rule.begin(); survive_num != survive_rule.end(); ++survive_num) {
		cout << (char)(*survive_num + CHAR_TO_NUM_COEF);
	}
	cout << '\n' << current_iteration << '\n';
	for (int i = 0; i < count; i++) {
		makeIteration(*this);
	}
	(*this).printMap();
}