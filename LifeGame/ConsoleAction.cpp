#include "ConsoleAction.h"

namespace ca = consoleaction;
namespace cs = console;
namespace gf = gamefield;

std::vector<ca::ConsoleAction*> ca::ConsoleAction::initialize_array() {
	std::vector<ConsoleAction*> output;
	output.push_back(new ca::Default);
	output.push_back(new ca::Dump);
	output.push_back(new ca::Tick);
	output.push_back(new ca::Exit);
	output.push_back(new ca::Help);
	return output;
}

void ca::ConsoleAction::doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument) {
	return;
}

void ca::Dump::doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument) {
	if (argument.length() == 0) {
		output.writeError(cs::NO_FILENAME);
		return;
	}
	field.dump(argument);
	return;
}

void ca::Help::doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument) {
	output.writeMessage(gf::HELP_MESSAGE);
	return;
}
void ca::Exit::doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument) {
	output.writeMessage(gf::EXIT_MESSAGE);
	endFlag = gf::END_OF_PROGRAMM;
	return;
}
void ca::Tick::doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument) {
	if (argument.length() == 0) {
		output.writeError(cs::NO_TICKS);
		return;
	}
	int ticks = std::stoi(argument);
	if (ticks <= 0) {
		output.writeError(cs::WRONG_TICKS);
		return;
	}
	field.iterate(ticks);
}
void ca::Default::doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument) {
	output.writeError(cs::WRONG_OPTION);
	return;
}