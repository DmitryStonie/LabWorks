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
	cs::Console console;
	console.printMap(field.return_map(), 15, 15);
	for (int i = 0; i < 100; i++) {
		field.iterate(1);
	}
	field.run();
	return 0;
}
