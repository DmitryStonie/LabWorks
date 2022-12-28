#include <iostream>
#include "ArgsContainer.h"
#include "Errors.h"

namespace ac = argscontainer;

ac::ArgsContainer::ArgsContainer() {
	filenames.resize(3);
	filenames[CONFIG_FILES].push_back(DEFAULT_CONFIG);
	filenames[INPUT_FILES].push_back(DEFAULT_INPUT);
	filenames[INPUT_FILES].push_back(DEFAULT_INPUT2);
	filenames[OUTPUT_FILES].push_back(DEFAULT_OUTPUT);
}

ac::ArgsContainer::~ArgsContainer() {

}

std::vector<std::vector<std::string>> ac::ArgsContainer::returnFilenames(int argc, char** argv) {
	//setDefaultArgs();
	po::options_description hidden;
	hidden.add_options()
		("outputFile", po::value<std::vector<std::string>>(&filenames[OUTPUT_FILES]), "Output file")
		("inputFile", po::value<std::vector<std::string>>(&filenames[INPUT_FILES]), "Input file")
		;
	po::options_description Allowed("Allowed options");
	Allowed.add_options()
		(",h", "Produces help message")
		(",c", po::value<std::vector<std::string>>(&filenames[CONFIG_FILES]), "Configuration file")
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
		std::set<std::string> checkRepeats(filenames[INPUT_FILES].begin(), filenames[INPUT_FILES].end());
		if (checkRepeats.size() != filenames[INPUT_FILES].size()) {
			throw errors::MULTIPLE_INPUT_FILES;
		}
		if (filenames[CONFIG_FILES].size() > NUM_OF_CONFIG_FILES) {
			throw errors::MULTIPLE_CONFIG_FILES;
		}
	}
	catch (const po::error& e) {
		throw errors::INVALID_COMMAND_LINE_ARGUMENTS;
	}
	catch (errors::errorCodes errCode) {
		throw errCode;
	}
	return filenames;
}
