#include "Console.h"

void ErrorOutput::writeToConsole(int errorId) {
	setlocale(LC_ALL, "Russian");
	cout << errors[errorId] << endl;
}

void ErrorOutput::writeToConsole(int errorId, string extraInformation) {

}

void Console::writeToConsole(string toWrite) {
	
}

ErrorOutput::ErrorOutput() {

}

ErrorOutput::~ErrorOutput() {

}
