#pragma once
#include <vector>
#include <string>
#include "Console.h"


using namespace std;

class GameField {
	vector<vector<char>> field;
	string universe_name;
	vector<int> survive_rule;
	vector<int> birth_rule;
	int width;
	int height;
	int input_rules(string source, GameField map);
public:
	GameField();
	GameField(string input_filename);
	~GameField();
	void makeIteration(GameField& map);
	void printMap(GameField& map);
};