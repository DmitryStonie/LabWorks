#pragma once
#include <boost/program_options.hpp>

namespace po = boost::program_options;

namespace argscontainer {
	const std::string DEFAULT_CONFIG = "defconf.txt";
	const std::string DEFAULT_INPUT = "definput.wav";
	const std::string DEFAULT_OUTPUT = "defoutput.wav";
	class ArgsContainer {
		std::string configFile;
		std::string outputFile;
		std::vector<std::string> inputFiles;
		po::options_description desc;
		po::positional_options_description pos_desc;
		po::variables_map var_map;
		po::variables_map pos_map;
		void setDefaultArgs();
	public:
		ArgsContainer();
		~ArgsContainer();
		ArgsContainer(int argc, char** argv);
		std::vector<std::string> returnArguments();
	};
}
