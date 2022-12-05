#pragma once
#include <string>
#include <iostream>
#include "GameField.h"

#include <boost/program_options.hpp>
#include <boost/filesystem/fstream.hpp>

namespace po = boost::program_options;
namespace fs = boost::filesystem;

using namespace std;

enum errorsCodes {
	MULTIPLE_ITERATIONS = 1,
	INCORRECT_ITERATIONS = 2,
	INCORRECT_INPUT_FILENAME = 3,
	INCORRECT_ITERATIONS_LONG = 4,
	INCORRECT_INPUT_FILENAME_LONG = 5,
	INCORRECT_PARAMETER = 6,
	NONINITIALIZED_ITERATIONS = 7,
	NONINITIALIZED_INPUT_FILENAME = 8,
	CANNOT_OPEN_FILE = 9,
	WRONG_FILE_FORMAT = 10,
	MULTIPLE_FILE_FORMAT = 11,
	WRONG_UNIVERSE_NAME = 12,
	MULTIPLE_UNIVERSE_NAME = 13,
	WRONG_RULES_FORMAT = 14,
	WRONG_BIRTH_RULE = 15,
	WRONG_SURVIVE_RULE = 16,
	MULTIPLE_RULES = 17,
	NO_UNIVERSE_NAME = 18,
	NO_GAME_RULES = 19,
	NO_FILE_FORMAT = 20,
	WRONG_BIRTH_RULE_FORMAT = 21,
	NO_SURVIVE_RULE = 22,
	WRONG_SYMBOLS = 23,
	MULTIPLE_BIRTH_RULE = 24,
	MULTIPLE_SURVIVAL_RULE = 25,
	WRONG_SURVIVAL_RULE_FORMAT = 26,
	MULTIPLE_ALIVE_CELL = 27,
	WRONG_RULE_FORMAT_DEFAULT = 28
};

class Console {
	string toOutput;
	po::options_description desc;
	po::positional_options_description pos_desc;
	po::variables_map var_map;
	po::variables_map pos_map;
public:
	Console();
	Console(int argc, char** argv);
	void writeToConsole(string toWtrite);
	void help();
	void gameFieldInitialization(GameField& map);
	//void start_interaction();

};

class ErrorOutput : public Console {
	string errors[100] = {
		"Отсутствие ошибки. Все хорошо!",
		"Ошибка : параметр - i указан несколько раз.Используется первое введенное значение - такая ошибка одна, но для четырех операторов",
		"Ошибка : для параметра - i некорректно передано число итераций.",
		"Ошибка : для параметра - o некорректно передано название выходного файла.",
		"Ошибка : для параметра --iterations = не указано число итераций",
		"Ошибка : для параметра --output = не указано название выходного файла.",
		"Ошибка : передан некорректный параметр.",
		"Ошибка : не указано число итераций.Установлено значение по умолчанию, равное",
		"Ошибка : не указано имя файла вывода.Установлено значение по умолчанию, равное",
		"Ошибка : невозможно открыть файл.",
		"Ошибка : неправильно указан формат файла",
		"Ошибка : формат файла указан более 1 раза.",
		"Ошибка : неправильно указано название вселенной.",
		"Ошибка : название вселенной указано больше 1 раза.Использовано первое значение.",
		"Ошибка : неправильный формат правил игры",
		"Ошибка : Неправильно указано правило рождения клеток",
		"Ошибка : Неправильно указано правило выживания клеток",
		"Ошибка : правила игры указаны больше 1 раза. Использовано первое значение.",
		"Ошибка : не указано название вселенной. Использовано значение по умолчанию.",
		"Ошибка : не указаны правила игры. Использованы значения по умолчанию.",
		"Ошибка : не указан формат файла.",
		"Ошибка : неправильный формат правила рождения клеток.",
		"Ошибка : не указано условие выживания клетки. Установлено значение по умолчанию.",
		"Ошибка : введена некорректная координата",
		"Ошибка : для правила рождения условия указаны несколько раз",
		"Ошибка : для правила выживания условия указаны несколько раз",
		"Ошибка : неправильный формат правила выживания клеток.",
		"Ошибка : координаты некоторой живой клетки были указаны несколько раз",
		"Ошибка : неправильный формат правил игры. Установлены зачения по умолчанию"
	};
public:
	ErrorOutput();
	~ErrorOutput();
	void writeToConsole(int errorId);
};
