#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>

namespace errors {
	enum errorCodes {
		CANNOT_OPEN_FILE = 0,
		UNSUPPORTED_FILE_FORMAT = 1,
		CORRUPTED_HEADER = 2,
		WRITE_ERROR = 3,
		READ_ERROR = 4,
		INVALID_COMMAND_LINE_ARGUMENTS = 5,
		MULTIPLE_INPUT_FILES = 6,
		MULTIPLE_CONFIG_FILES = 7,
		INVALID_CONFIG_FILE = 8
	};
	class ErrorOutput {
		std::map<int, const std::string> errors = {
			{CANNOT_OPEN_FILE,"Error: can't open file"},
			{UNSUPPORTED_FILE_FORMAT,"Error: Header is not a WAV header. Can't use this file\n"},
			{CORRUPTED_HEADER,"Error: Header is corrupted\n"},
			{WRITE_ERROR,"Error: Cannot write data to file\n"},
			{READ_ERROR,"Error: Cannot read data to file\n"},
			{INVALID_COMMAND_LINE_ARGUMENTS, "Error: Invalid Command line arguments\n"},
			{MULTIPLE_INPUT_FILES, "Error: some files are used more than 1 time\n"},
			{MULTIPLE_CONFIG_FILES, "Error: used more than 1 config files\n"},
			{INVALID_CONFIG_FILE, "Error: config file has wrong data\n"}
		};
	public:
		ErrorOutput();
		~ErrorOutput();
		void writeError(int errorCode);
	};
}