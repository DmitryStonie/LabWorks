#include "Configparser.h"

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
	int first = 0, last, delta;
	delta = line.find(DELIMETER);
	for (; delta != std::string::npos;) {
		argument = line.substr(first);
		delta = line.find(DELIMETER, first);
		arguments.push_back(line.substr(first, delta - first));
		first += delta;
		delta = line.find(DELIMETER, first);
	}
	argList.push_back(arguments);
}
cp::ConfigParser::ConfigParser() {

}

cp::ConfigParser::ConfigParser(std::string filename, std::vector<std::string> converters) {
	const char LATTICE_SYMBOL = '#';
	std::ifstream input(filename);
	if (!(input.is_open())) {
		std::cout << "error\n";
	}
	std::string line;//try catch
	std::getline(input, line, '\n');
	for (; !(input.eof());) {
		getline(input, line);
		if (line[0] == LATTICE_SYMBOL);//comment
		else if (find_converter(line, converters) == FOUND) {
			write_arguments(line);
		}
	}

}
cp::ConfigParser::~ConfigParser() {

}
std::vector<std::vector<std::string>> cp::ConfigParser::return_config() {
	return argList;
}