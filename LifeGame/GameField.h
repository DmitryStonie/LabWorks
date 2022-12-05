#pragma once
#include <vector>
#include <string>
#include "Console.h"
#include <set> 


using namespace std;

class GameField {
	vector<vector<char>> field;
	string universe_name;
	set<int> survive_rule;
	set<int> birth_rule;
	int width;
	int height;
	int input_rules(string source, GameField &map);
	void input_header(ifstream& inf, GameField &map);
	void input_cells(ifstream& inf, GameField &map);
	void calculate_size(ifstream& inf, GameField& map);
	inline int count_center_neighbours(int x, int y, GameField& map);
	inline int count_border_neighbours(int x, int y, GameField& map);
	inline void update_center_cell(int x, int y, GameField& map, GameField& tmp_map);
	inline void update_border_cell(int x, int y, GameField& map, GameField& tmp_map);

public:
	GameField();
	GameField(string input_filename);
	GameField& operator=(const GameField &a);
	~GameField();
	void makeIteration(GameField& map);
	void printMap();
	void dump(string filename);
	void iterate(int current_iteration, int count);
};