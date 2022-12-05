#include "Console.h"
#include <map>

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

void ErrorOutput::writeToConsole(int errorId, string extraInformation) {

}

void Console::writeToConsole(string toWrite) {
	
}

ErrorOutput::ErrorOutput() {

}

ErrorOutput::~ErrorOutput() {

}

void Console::help() {
	cout << "Это сообщение help";
}

void Console::start_interaction() {
	string input;
	for (;;) {
		getline(cin, input);
		/*switch (input) {
		case DUMP_STR:
				dump();
				break;

		}*/
	}
}
