#include "ConfigParser.h"
#include "Errors.h"


namespace cp = configparser;

int cp::ConfigParser::find_converter(std::string line, std::vector<std::string> converters) {
	const int FIRST_INDEX = 0;
	for (int i = 0; i < converters.size(); i++) {
		if (line.find(converters[i]) == FIRST_INDEX) return FOUND;
	}
}

void cp::ConfigParser::write_arguments(std::string line) {
	std::vector<std::string> arguments;
	std::string DELIMETER = " ", argument;
	line[line.size() - 1] = ' ';	//change \n to DELIMETER
	const int DELIMETER_LENGTH = 1;
	int first = 0, last, pos;
	for (; (pos = line.find(DELIMETER)) != std::string::npos;) {
		argument = line.substr(0,pos);
		arguments.push_back(argument);
		line.erase(0, pos + DELIMETER_LENGTH);
	}
	argList.push_back(arguments);
}
cp::ConfigParser::ConfigParser() {

}

cp::ConfigParser::~ConfigParser() {

}

void cp::ConfigParser::initialize(std::string filename, std::vector<std::string> converters) {
	const char LATTICE_SYMBOL = '#';
	std::ifstream input(filename, std::ios_base::in);
	if (!(input.is_open())) {
		throw errors::CANNOT_OPEN_FILE;
	}
	std::string line;//try catch
	for (; !(input.eof());) {
		std::getline(input, line, '\n');
		//if (line[0] == LATTICE_SYMBOL); LATTICE SYMBOL and other symbols is allowed
		if (find_converter(line, converters) == FOUND) {
			write_arguments(line);
		}
	}

}

std::vector<std::vector<std::string>> cp::ConfigParser::return_config() {
	return argList;
}