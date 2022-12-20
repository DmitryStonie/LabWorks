#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>

namespace configparser {
	enum findcond {
		FOUND = 0,
		NOT_FOUND = 1
	};
	class ConfigParser {
		std::vector<std::vector<std::string>> argList;
		int find_converter(std::string line, std::vector<std::string> converters);
		void write_arguments(std::string line);
	public:
		ConfigParser();
		ConfigParser(std::string filename, std::vector<std::string> converters);
		~ConfigParser();
		std::vector<std::vector<std::string>> return_config();
	};
}