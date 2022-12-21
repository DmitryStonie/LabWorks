#pragma once
#include <vector>
#include <string>
#include <limits>
#include "WavFile.h"

namespace converter {
	const std::string MIX_STR = "mix";
	const std::string MUTE_STR = "mute";
	const std::string LOWER_STR = "lower";
	const int MAX_DATA_SIZE = UINT32_MAX;
	const int NO_SECOND_STREAM = -1;
	const int NOT_OPENED = 1;
	const int ZERO = 0;
	enum conv_codes {
		MIX_CONVERTER = 0,
		MUTE_CONVERTER = 1,
		LOWER_CONVERTER = 2,
		NOT_CONVERTER = 3
	};
	class Converter {
	protected:
		int interval_start;
		int interval_end;
		int secondStreamNumber;
	public:
		virtual void convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int interval_start, int interval_end);
		virtual void initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond);
		virtual int secondInputArg();
		virtual ~Converter() {}
	};

	class Mix : public Converter {
	public:
		void convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int interval_start, int interval_end);
		void initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond);
		int secondInputArg();
	};

	class Mute : public Converter {
	public:
		void convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int interval_start, int interval_end);
		void initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond);
		int secondInputArg();
	};

	class Lower : public Converter {
	public:
		void convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int interval_start, int interval_end);
		void initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond);
		int secondInputArg();
	};

	class ConverterFactory {
	public:
		virtual Converter* factoryMethod();
		std::vector<ConverterFactory*> initialize_array();
		virtual ~ConverterFactory();
	};

	class MixFactory : public ConverterFactory{
		Converter* factoryMethod();
	};

	class MuteFactory : public ConverterFactory{
		Converter* factoryMethod();

	};

	class LowerFactory : public ConverterFactory {
		Converter* factoryMethod();	
	};

	class SoundProcessor {
		std::vector<Converter*> converters;
		std::vector<wavfile::WavFile*> files;
		std::vector<unsigned short> input1;
		std::vector<unsigned short> input2;
		std::vector<unsigned short> output;
		int convFind(std::string& convToFind, std::vector<std::string>& converterNames);
		void init_files(std::vector<std::string>& filenames);
		void init_converters(std::vector<std::vector<std::string>>& config);
		int countMaxSize();
	public:
		SoundProcessor();
		~SoundProcessor();
		void initialize(std::vector<std::vector<std::string>>& config, std::vector<std::string>& filenames);
		void run(std::vector<std::string>& fileNames);

	};
}
