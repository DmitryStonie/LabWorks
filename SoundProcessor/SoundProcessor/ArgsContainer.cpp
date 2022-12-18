#include "ArgsContainer.h"
#include <iostream>

namespace ac = argscontainer;

ac::ArgsContainer::ArgsContainer() {
	setDefaultArgs();
}

ac::ArgsContainer::~ArgsContainer() {

}

ac::ArgsContainer::ArgsContainer(int argc, char** argv) {
	setDefaultArgs();
	po::options_description hidden;
	hidden.add_options()
		("outputFile", po::value<std::string>(&outputFile), "Output file")
		("inputFile", po::value<std::vector<std::string>>(&inputFiles), "Input file")
		;
	po::options_description Allowed("Allowed options");
	Allowed.add_options()
		(",h", "Produces help message")
		(",c", po::value<std::string>(&configFile), "Configuration file")
		;
	po::options_description descriptor;
	descriptor.add(Allowed).add(hidden);
	po::positional_options_description pos_descriptor;
	pos_descriptor.add("outputFile", 1);
	pos_descriptor.add("inputFile", -1);
	po::variables_map var_map;
	try {
		po::store(po::command_line_parser(argc, argv).options(descriptor).positional(pos_descriptor).run(), var_map);
		po::notify(var_map);
		if (var_map.count("-h")) {
			std::cout << Allowed << '\n';
		}
		if (var_map.count("outputFile")) {
			std::cout << "Input files are: " << var_map["outputFile"].as< std::string> () << "\n";
		}
		if (var_map.count("inputFile")) {
			std::cout << "Input files are: ";
			inputFiles = var_map["inputFile"].as< std::vector<std::string>>();
			for (int i = 0; i < inputFiles.size(); i++) {
				std::cout << inputFiles[i] << "\n";
			}
		}
	}
	catch (const po::error& e) {
		std::cout << "Error. Invalid command line arguments\n";
	}
}

std::vector<std::string> ac::ArgsContainer::returnArguments() {
	std::vector<std::string> arguments(inputFiles);
	arguments.push_back(outputFile);
	arguments.push_back(configFile);
	return arguments;
}

void ac::ArgsContainer::setDefaultArgs() {
	configFile = DEFAULT_CONFIG;
	outputFile = DEFAULT_OUTPUT;
	inputFiles.resize(0);
	inputFiles.push_back(DEFAULT_INPUT);
}
