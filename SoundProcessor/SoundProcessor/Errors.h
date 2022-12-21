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
		INVALID_COMMAND_LINE_ARGUMENTS = 5
	};
	class ErrorOutput {
		std::map<int, const std::string> errors = {
			{CANNOT_OPEN_FILE,""},
			{UNSUPPORTED_FILE_FORMAT,"Header is not a WAV header. Can't use this file\n"},
			{CORRUPTED_HEADER,"Header is corrupted\n"},
			{WRITE_ERROR,"Cannot write data to file\n"},
			{READ_ERROR,"Cannot read data to file\n"},
			{INVALID_COMMAND_LINE_ARGUMENTS, "Error. Invalid Command line arguments\n"},
		};
	public:
		ErrorOutput();
		~ErrorOutput();
		void writeError(int errorCode);
	};
}