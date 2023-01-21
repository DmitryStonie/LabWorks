#include <iostream>
#include <string>
#include "GameField.h"
#include "Console.h"

namespace cs = console;
namespace gf = gamefield;

int main(int argc, char* argv[])
{
	gf::ArgsContainer inputf(argc, argv);
	gf::GameField gamefield;
	inputf.gameFieldInitialization(gamefield);
	gamefield.run();
	return 0;
}
