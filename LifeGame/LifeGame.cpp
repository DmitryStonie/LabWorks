#include <iostream>
#include <string>
#include "GameField.h"
#include "Console.h"
#include "ArgsContainer.h"

namespace gf = gamefield;
namespace ac = argscontainer;
namespace cs = console;

int main(int argc, char* argv[])
{
	gf::GameField field;
	ac::ArgsContainer arguments(argc, argv);
	arguments.gameFieldInitialization(field);
	field.run();
	return 0;
}
