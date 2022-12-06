#include "InputArgs.h"

namespace po = boost::program_options;
namespace fs = boost::filesystem;
using namespace argscontainer;

ArgsContainer::ArgsContainer() {

}

ArgsContainer::ArgsContainer(int argc, char** argv) {
	gamefield::GameField tmp_map;
	po::options_description desc("Allowed options:");
	desc.add_options()
		("help", "Writes help message")
		("iterations,i", po::value<int>(&tmp_map.iterations)->default_value(DEFAULT_ITERATIONS), "Number of universe iterations")
		("input", po::value<string>(&tmp_map.input_file)->default_value(DEFAULT_INPUT_FILENAME), "Name of file for input")
		("output,o", po::value<string>(&tmp_map.output_file)->default_value(DEFAULT_OUTPUT_FILENAME), "Name of file for output")
		;

	po::variables_map var_map;
	po::store(po::parse_command_line(argc, argv, desc), var_map);
	po::notify(var_map);
	if (var_map.count("help")) {
		std::cout << desc << '\n';
	}

	po::positional_options_description desc_pos;
	desc_pos.add("input", -1);

	po::variables_map pos_map;
	po::store(po::command_line_parser(argc, argv).options(desc).positional(desc_pos).run(), pos_map);
	po::notify(pos_map);
}

ArgsContainer::~ArgsContainer() {

}

void ArgsContainer::gameFieldInitialization(gamefield::GameField& map) {

}