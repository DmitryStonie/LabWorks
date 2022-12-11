#include "Parser.h"
@include <iostream>
#include <cstdio>

parser::Header::Header() {

}

parser::Header::Header(std::string filename) {
	FILE* input_file;
	errno_t error;
	error = fopen_s(&input_file, "severe_tire_damage.wav", "rb");
	if (error) {
		std::cerr << "Unable to open file, error " << error << '\n';
	}
	//default
	char buf[44];
	int read_elem = 0;
	read_elem = fread_s(buf, sizeof(char), HEADER_SIZE, sizeof(char), HEADER_SIZE, input_file);
	if (read_elem != HEADER_SIZE) {
		std::cerr << "Header is corrupted.\n";
	}

}

parser::Header::~Header() {

}