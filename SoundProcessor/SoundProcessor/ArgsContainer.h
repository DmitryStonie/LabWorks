#pragma once
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace argscontainer {
	const std::string DEFAULT_CONFIG = "defconfig.txt";
	const std::string DEFAULT_INPUT = "definput1.wav";
	const std::string DEFAULT_INPUT2 = "definput2.wav";
	const std::string DEFAULT_OUTPUT = "defoutput.wav";
	enum filepositions {
		CONFIG_FILES = 0,
		INPUT_FILES = 1,
		OUTPUT_FILES = 2
	};
	const int CONFIG_POS = 0;
	const int OUTPUT_POS = 0;
	class ArgsContainer {
		std::vector<std::vector<std::string>> filenames;
	public:
		ArgsContainer();
		~ArgsContainer();
		std::vector<std::vector<std::string>> returnFilenames(int argc, char** argv);
	};
}
