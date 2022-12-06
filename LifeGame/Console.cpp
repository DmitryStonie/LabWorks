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
		{CANNOT_OPEN_FILE, "Ошибка : невозможно открыть файл."},
		{MULTIPLE_FILE_FORMAT, "Ошибка : неправильно указан формат файла"},
		{WRONG_UNIVERSE_NAME, "Ошибка : неправильно указано название вселенной."},
		{MULTIPLE_UNIVERSE_NAME, "Ошибка : название вселенной указано больше 1 раза.Использовано первое значение."},
		{WRONG_RULES_FORMAT, "Ошибка : неправильный формат правил игры"},
		{NO_SURVIVE_RULE,"Ошибка : не указано условие выживания клетки. Установлено значение по умолчанию."},
		{MULTIPLE_RULES,"Ошибка : правила игры указаны больше 1 раза. Использовано первое значение."},
		{NO_FILE_FORMAT,"Ошибка : не указан формат файла."},
		{NO_UNIVERSE_NAME,"Ошибка : не указано название вселенной. Использовано значение по умолчанию."},
		{NO_GAME_RULES,"Ошибка : не указаны правила игры. Использованы значения по умолчанию."},
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