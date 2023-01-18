#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <set>

namespace configparser {
	enum findcond {
		FOUND = 0,
		NOT_FOUND = 1
	};
	class ConfigParser {
		std::vector<std::vector<std::string>> argList;
		int find_converter(const std::string line, std::vector<std::string> converters);
		void write_arguments(std::string line);
	public:
		ConfigParser();
		~ConfigParser();
		void initialize(std::vector<std::string> filename, std::set<std::string> converters);
		std::vector<std::vector<std::string>> return_config();
	};
}