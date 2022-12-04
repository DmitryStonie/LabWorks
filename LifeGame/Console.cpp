#include "Console.h"
#include <map>

void ErrorOutput::writeToConsole(int errorId) {
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
