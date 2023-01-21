#include "ConfigParser.h"
#include "Errors.h"

namespace cp = configparser;

int cp::ConfigParser::find_converter(std::string line, std::vector<std::string> converters) {
	const int FIRST_INDEX = 0;
	for (int i = 0; i < converters.size(); i++) {
		if (line.find(converters[i]) == FIRST_INDEX) return FOUND;
	}
}

void cp::ConfigParser::write_arguments(const std::string line) {
	std::vector<std::string> arguments;
	std::string line_copy = line + ARG_DELIMETER;
	std::string argument;
	const int DELIMETER_LENGTH = 1;
	int index = line_copy.find(ARG_DELIMETER), last_index = -1, arg_length = 0;
	for (; index != std::string::npos;) {
		arg_length = index - last_index - 1;
		argument = line_copy.substr(last_index + 1, arg_length);
		arguments.push_back(argument);
		last_index = index;
		index = line_copy.find(ARG_DELIMETER, index + 1);
	}
	argList.push_back(arguments);
}
cp::ConfigParser::ConfigParser() {

}

cp::ConfigParser::~ConfigParser() {

}

void cp::ConfigParser::initialize(const std::vector<std::string> filename, const std::set<std::string> converters) {
	std::ifstream input(filename[0], std::ios_base::in);
	if (!(input.is_open())) {
		throw errors::CANNOT_OPEN_FILE;
	}
	std::string line, name;
	for (; !(input.eof());) {
		std::getline(input, line, LINE_DELIMETER);
		name = line.substr(0, line.find(ARG_DELIMETER));
		if (converters.find(name) == converters.end() && line[0] != LATTICE_SYMBOL) {
			throw errors::INVALID_CONFIG_FILE;
		}
		else if (converters.find(name) != converters.end()) {
			write_arguments(line);
		}
	}
}

std::vector<std::vector<std::string>> cp::ConfigParser::return_config() {
	return argList;
}