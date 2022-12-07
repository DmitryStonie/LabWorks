#include <iostream>
#include <string>
#include "GameField.h"
#include "Console.h"

namespace gf = gamefield;
namespace cs = console;

int main(int argc, char* argv[])
{
	gf::GameField field;
	gf::ArgsContainer arguments(argc, argv);
	arguments.gameFieldInitialization(field);
	field.run();
	return 0;
}
