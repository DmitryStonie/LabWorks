#include "Console.h"
#include "GameField.h"
#include <map>


const int DEFAULT_ITERATIONS = 10;
const string DEFAULT_OUTPUT_FILENAME = string("a.txt");
const string DEFAULT_INPUT_FILENAME = string("default.txt");

Console::Console(){

}

Console::~Console() {

}

void Console::writeError(int errorId) {
	setlocale(LC_ALL, "Russian");
	map<int, string> errors = {
		{CANNOT_OPEN_FILE, "������ : ���������� ������� ����."},
		{MULTIPLE_FILE_FORMAT, "������ : ����������� ������ ������ �����"},
		{WRONG_UNIVERSE_NAME, "������ : ����������� ������� �������� ���������."},
		{MULTIPLE_UNIVERSE_NAME, "������ : �������� ��������� ������� ������ 1 ����.������������ ������ ��������."},
		{WRONG_RULES_FORMAT, "������ : ������������ ������ ������ ����"},
		{NO_SURVIVE_RULE,"������ : �� ������� ������� ��������� ������. ����������� �������� �� ���������."},
		{MULTIPLE_RULES,"������ : ������� ���� ������� ������ 1 ����. ������������ ������ ��������."},
		{NO_FILE_FORMAT,"������ : �� ������ ������ �����."},
		{NO_UNIVERSE_NAME,"������ : �� ������� �������� ���������. ������������ �������� �� ���������."},
		{NO_GAME_RULES,"������ : �� ������� ������� ����. ������������ �������� �� ���������."},
	};
	cerr << errors[errorId] << endl;
}

void Console::printMap(GameField& map) {
	for (int i = 0; i < map.height; i++) {
		for (int j = 0; j < map.width; j++) {
			if (map.field[i][j] == ALIVE_CELL) {
				cout << 'O' << ' ';
			}
			else {
				cout << '#' << ' ';
			}
		}
		cout << '\n';
	}
}

void Console::dump(GameField& map) {
	ofstream fout(map.input_file);
	fout << FILE_FORMAT << '\n' << UNIVERSE_NAME_SPEC << map.universe_name << '\n' << RULES_SPEC << BIRTH_LETTER;
	for (set<int>::iterator birth_num = map.birth_rule.begin(); birth_num != map.birth_rule.end(); ++birth_num) {
		fout << (char)(*birth_num + CHAR_TO_NUM_COEF);
	}
	fout << SLASH << SURVIVE_LETTER;
	for (set<int>::iterator survive_num = map.survive_rule.begin(); survive_num != map.survive_rule.end(); ++survive_num) {
		fout << (char)(*survive_num + CHAR_TO_NUM_COEF);
	}
	fout << '\n';
	for (int i = 0; i < map.height; i++) {
		for (int j = 0; j < map.width; j++) {
			if (map.field[i][j] == ALIVE_CELL) {
				fout << i << " " << j << '\n';
			}
		}
	}
	fout.close();
}