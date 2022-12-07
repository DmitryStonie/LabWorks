#include "Console.h"
#include <map>
#include <fstream>

const int DEFAULT_ITERATIONS = 10;
const int ALIVE_CELL = 1;
const std::string DEFAULT_OUTPUT_FILENAME = std::string("a.txt");
const std::string DEFAULT_INPUT_FILENAME = std::string("default.txt");

console::Console::Console(){

}

console::Console::~Console() {

}

void console::Console::writeError(int errorId) {
	setlocale(LC_ALL, "Russian");
	std::map<int, std::string> errors = {
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
	std::cerr << errors[errorId] << std::endl;
}

void console::Console::printMap(std::vector<std::vector<int>> field, int height, int width) {
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (field[i][j] == ALIVE_CELL) {
				std::cout << 'O' << ' ';
			}
			else {
				std::cout << '#' << ' ';
			}
		}
		std::cout << '\n';
	}
}

int console::Console::read_command(std::string &argument) {
	std::string input, tmp;
	std::cin >> input;
	tmp = input.substr(0, COMMAND_LENGTH);
	if (tmp == DUMP_STR) {
		if (input.find(LEFTARROW, 0) == std::string::npos || input.find(RIGHTARROW, 0) == std::string::npos) return WRONG_COMMAND;
		else argument = input.substr(input.find(LEFTARROW, 0) + 1, input.find(RIGHTARROW, 0) - input.find(LEFTARROW, 0) - 1);
		return DUMP;
	}
	else if(tmp == HELP_STR) {
		return HELP;
	}
	else if(tmp == EXIT_STR) {
		return EXIT;
	}
	else if (tmp == TICK_STR || tmp == T_STR) {
		if (input.find(LEFTARROW, 0) == std::string::npos || input.find(RIGHTARROW, 0) == std::string::npos) return WRONG_COMMAND;
		else argument = input.substr(input.find(LEFTARROW, 0) + 1, input.find(RIGHTARROW, 0) - input.find(LEFTARROW, 0) - 1);
		return TICK;
	}
	else {
		return WRONG_COMMAND;
	}
}
