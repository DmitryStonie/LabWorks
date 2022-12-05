#include "Console.h"
#include <map>


const int DEFAULT_ITERATIONS = 10;
const string DEFAULT_OUTPUT_FILENAME = string("a.txt");
const string DEFAULT_INPUT_FILENAME = string("default.txt");

Console::Console(){}
Console::Console(int argc, char** argv) {
	GameField tmp_map;
	po::options_description desc("Allowed options:");
	desc.add_options()
		("help", "Writes help message")
		("iterations,i", po::value<int>(&tmp_map.iterations)->default_value(DEFAULT_ITERATIONS), "Number of universe iterations")
		("input", po::value<string>(&tmp_map.input_file)->default_value(DEFAULT_INPUT_FILENAME), "Name of file for input")
		("output,o", po::value<string>(&tmp_map.output_file)->default_value(DEFAULT_OUTPUT_FILENAME), "Name of file for output")
		;

	po::variables_map var_map;
	po::store(po::parse_command_line(argc, argv, desc), var_map);
	po::notify(var_map);
	if (var_map.count("help")) {
		cout << desc << '\n';
	}

	po::positional_options_description desc_pos;
	desc_pos.add("input", -1);

	po::variables_map pos_map;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(desc_pos).run(), pos_map);
	po::notify(pos_map);
}

void Console::writeToConsole(string toWrite) {
	
}

void ErrorOutput::writeToConsole(int errorId) {
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
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{},
		{}
	};
	cerr << errors[errorId] << endl;
}

ErrorOutput::ErrorOutput(){}

ErrorOutput::~ErrorOutput(){}