#pragma once
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace argscontainer {
	const std::string DEFAULT_CONFIG = "defconf.txt";
	const std::string DEFAULT_INPUT = "definput.wav";
	const std::string DEFAULT_INPUT2 = "definput2.wav";
	const std::string DEFAULT_OUTPUT = "defoutput.wav";
	class ArgsContainer {
		std::string configFile;
		std::string outputFile;
		std::vector<std::string> inputFiles;
		void setDefaultArgs();
	public:
		ArgsContainer();
		~ArgsContainer();
		ArgsContainer(int argc, char** argv);
		std::vector<std::string> returnArguments();
	};
}
