#pragma once
#include "GameField.h"
#include "Console.h"
#include <vector>

namespace consoleaction {
	class ConsoleAction : public gamefield::GameField, public console::Console {
	public:
		virtual void doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument);
		std::vector<ConsoleAction*> initialize_array();
	};

	class Dump : public ConsoleAction {
		void doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument);
	};
	class Help : public ConsoleAction {
		void doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument);
	};
	class Exit : public ConsoleAction {
		void doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument);
	};
	class Tick : public ConsoleAction {
		void doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument);
	};
	class Default : public ConsoleAction {
		void doAction(gamefield::GameField field, console::Console output, int &endFlag, std::string argument);
	};

}