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
