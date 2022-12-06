#pragma once
#include <string>
#include <iostream>
#include <map>
#include "GameField.h"
#include <boost/program_options.hpp>
#include <boost/filesystem/fstream.hpp>

namespace po = boost::program_options;

namespace argscontainer {
	const int DEFAULT_ITERATIONS = 10;
	const std::string DEFAULT_OUTPUT_FILENAME = string("defaultOutput.txt");
	const std::string DEFAULT_INPUT_FILENAME = string("defaultInput.txt");

	class ArgsContainer {
		gamefield::GameField buf_field;
		po::options_description desc;
		po::positional_options_description pos_desc;
		po::variables_map var_map;
		po::variables_map pos_map;
	public:
		ArgsContainer();
		ArgsContainer(int argc, char** argv);
		~ArgsContainer();
		void gameFieldInitialization(gamefield::GameField& map);
	};
}