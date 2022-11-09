#include <vector>
#include "GameField.h"
#include "Console.h"
#include <fstream>
#include <bitset>

#define DEFAULT_UNIVERSE_NAME string("DefaultUniverseName")
#define DEFAULT_SURVIVE_RULE vector<int>{2, 3}
#define DEFAULT_BIRTH_RULE vector<int>{3}
#define DEFAULT_WIDTH 72
#define DEFAULT_HEIGHT 48
#define NON_CRITICAL_ERROR 2
#define CRITICAL_ERROR 3
#define ALIVE_CELL 1
#define DEAD_CELL 0

using namespace std;

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
	width = DEFAULT_WIDTH;
	height = DEFAULT_HEIGHT;
	if (!inf) {
		errout.writeToConsole(CANNOT_OPEN_FILE);
		field.resize(height, vector<char>(width, DEAD_CELL));
		survive_rule = DEFAULT_SURVIVE_RULE;
		birth_rule = DEFAULT_BIRTH_RULE;
	}
	else {
		string strInput;
		int is_format = 0, is_name = 0, is_rule = 0, line_index = 0;
		for (; line_index < 3; line_index++) {
			getline(inf, strInput);
			if (strInput.substr(0, 2) == "#L") {
				if (strInput != string("#Life 1.06")) {
					errout.writeToConsole(WRONG_FILE_FORMAT);
				}
				else {
					if (is_format == 0) is_format = 1;
					else {
						errout.writeToConsole(MULTIPLE_FILE_FORMAT);
					}
				}
			} else if (strInput.substr(0, 2) == "#N ") {
				if (strInput.size() == 3) {
					errout.writeToConsole(WRONG_UNIVERSE_NAME);
				}
				if (is_name == 0) {
					is_name = 1;
					universe_name = strInput.substr(3);
				}
				else {
					errout.writeToConsole(MULTIPLE_UNIVERSE_NAME);
				}
			} else if (strInput.substr(0, 2) == "#R ") { // #R B[0-7]*8/S[0-7]*8
				if (strInput.size() == 3){
					errout.writeToConsole(WRONG_RULES_FORMAT);
				}
				if (is_format == 0) {//not sure
					if (input_rules(strInput, *this) == NON_CRITICAL_ERROR) {
						is_format = 1;
					}
				}
				else {	//if file is wrong
					errout.writeToConsole(MULTIPLE_RULES);
				}
			}
			else {
				errout.writeToConsole(WRONG_FILE_FORMAT);
				break;
			}
		}
		if (is_format == 0) {
			errout.writeToConsole(NO_FILE_FORMAT);
		} 
		if (is_name == 0) {
			errout.writeToConsole(NO_UNIVERSE_NAME);
			universe_name = DEFAULT_UNIVERSE_NAME;
		}
		if (is_rule == 0) {
			errout.writeToConsole(NO_GAME_RULES);
			survive_rule = DEFAULT_SURVIVE_RULE;
			birth_rule = DEFAULT_BIRTH_RULE;
		}
		//right size
		int current_pos = inf.tellg();
		int x_cor = 0, y_cor = 0;
		for (; getline(inf, strInput);) {
			sscanf(strInput.data(), "%d %d", &x_cor, &y_cor);
			x_cor = abs(x_cor);
			y_cor = abs(y_cor);
			if (x_cor > width) {
				width = x_cor;
			}
			if (y_cor > height) {
				height = y_cor;
			}
		}
		//need to make pointer back
		field.assign(height, vector<char>(width, DEAD_CELL));
		inf.seekg(current_pos);
		//coordinates
		for (; getline(inf, strInput);) {
			sscanf(strInput.data(), "%d %d", &x_cor, &y_cor);
			if (x_cor < 0) x_cor = width + x_cor;
			if (y_cor < 0) y_cor = height + y_cor;
			field[y_cor][x_cor] = ALIVE_CELL;
		}
	}
	inf.close();
}

GameField::~GameField() {

}

int GameField::input_rules(string source, GameField map) {	// #R B[0-7]*8/S[0-7]*8
	ErrorOutput errout;
	map.birth_rule.resize(9, 0);
	int pos = 3;
	if (source[pos] != 'B') {
		errout.writeToConsole(WRONG_BIRTH_RULE);
		return CRITICAL_ERROR;
	}
	pos++;
	int incorrect_number = 0;
	for (; pos < source.length(); pos++) {
		if (source[pos] >= '0' && source[pos] <= '8') {
			if (map.birth_rule[source[pos] - 48] != 0) {
				incorrect_number = 1;
			}
			else {
				map.birth_rule[source[pos] - 48] = 1;
			}
		}
		else if(source[pos] == '\\') {
			pos++;
			break;
		}
		else {
			incorrect_number = 2;
		}
	}
	if (incorrect_number == 1) {
		errout.writeToConsole(MULTIPLE_BIRTH_RULE);
	}
	else if (incorrect_number == 2) {
		errout.writeToConsole(WRONG_BIRTH_RULE_FORMAT);
	}
	incorrect_number = 0;
	if (pos == source.length()) {
		errout.writeToConsole(NO_SURVIVE_RULE);
		map.survive_rule = DEFAULT_SURVIVE_RULE;
		return CRITICAL_ERROR;
	}
	for (; pos < source.length(); pos++) {
		if (source[pos] >= '0' && source[pos] <= '8') {
			if (map.survive_rule[source[pos] - 48] != 0) {
				incorrect_number = 1;
			}
			else {
				map.survive_rule[source[pos] - 48] = 1;
			}
		}
		else if (source[pos] == '\\') {
			pos++;
			break;
		}
		else {
			incorrect_number = 2;
		}
	}
	if (incorrect_number == 1) {
		errout.writeToConsole(MULTIPLE_SURVIVAL_RULE);
	}
	else if (incorrect_number == 2) {
		errout.writeToConsole(WRONG_SURVIVAL_RULE_FORMAT);
	}
}

void makeIteration(GameField& map) {

}

void GameField::printMap(GameField& map) {
	char symbol;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (map.field[i][j] == ALIVE_CELL) {
				cout << 'O';
			} else {
				cout << '#';
			}
		}
		cout << '\n';
	}
}