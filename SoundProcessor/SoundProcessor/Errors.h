#pragma once
#include <string>
#include <iostream>
#include <vector>
#include <map>

namespace errors {
	enum errorCodes {

	};
	class ErrorOutput {
		std::map<int, const std::string> errors = {
			{CANNOT_OPEN_FILE,""},
			{UNSUPPORTED_HEADER,""},
			{,},
			{,},
			{,},
			{,},
			{,},
			{,}
		};
	public:
		ErrorOutput();
		~ErrorOutput();
		void writeError(int errorCode);
	};
}