#include "ArgsContainer.h"
#include <iostream>

namespace ac = argscontainer;

ac::ArgsContainer::ArgsContainer() {

}

ac::ArgsContainer::~ArgsContainer() {

}

ac::ArgsContainer::ArgsContainer(int argc, char** argv) {
	std::string config_file;
	po::options_description visible_options("Allowed options:");
	desc.add_options()
		("config,c", po::value<std::string>(&config_file)->default_value(ac::DEFAULT_CONFIG), "Number of universe iterations")
		("output_file", po::value<std::string>(&config_file[1])->default_value(ac::DEFAULT_OUTPUT), "Number of universe iterations")
		("output_file", po::value<std::string>(&config_file[1])->default_value(ac::DEFAULT_OUTPUT), "Number of universe iterations")
		;
	po::positional_options_description soundfiles("Hidden options");
	soundfiles.add("output_file", 1).add("input_files", -1);

	po::variables_map var_map;
	po::store(po::parse_command_line(argc, argv, desc), var_map);
	po::notify(var_map);
	if (var_map.count(",h")) {
		std::cout << desc << '\n';
	}
}
