#pragma once
#include <vector>
#include <set>
#include <string>
#include <limits>
#include "WavFile.h"

namespace converter {
	const std::vector<std::string> CONVERTER_NAMES = { "mix", "mute", "changebeats" };
	const std::set<std::string> CONVERTER_NAMES_SET = { "mix", "mute", "changebeats" };
	const int MAX_DATA_SIZE = UINT32_MAX;
	const int NO_SECOND_STREAM = -1;
	const int NOT_OPENED = 1;
	const int ZERO = 0;
	const int FIRST_FILE = 1;
	const int PARAMETERS_NUM = 3;
	enum conv_codes {
		MIX_CONVERTER = 0,
		MUTE_CONVERTER = 1,
		MULTIPLE_CONVERTER = 2,
		NOT_CONVERTER = 3
	};
	enum stream_codes {
		INPUT_STREAM = 0,
		OUTPUT_STREAM = 1
	};
	class Converter {
	private:
		virtual void countBorders(int& start_index, int& end_index, int stream_start, int stream_end, int buf_size, int elem_size);
	protected:
		int interval_start;
		int interval_end;
		int secondStreamNumber;
	public:
		Converter();
		virtual void convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int interval_start, int interval_end);
		virtual void initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond);
		int secondInputArg();
		virtual int calculateReadPos(const int readPos, int bytesPerSecond);
		virtual ~Converter() {}
		virtual std::string returnInfo();
	};

	const std::string returnHelpMessage();

	class Mix : public Converter {
	private:
		void countBorders(int& start_index, int& end_index, int stream_start, int stream_end, int buf_size, int elem_size);
	public:
		void convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int interval_start, int interval_end);
		void initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond);
		int calculateReadPos(const int readPos, int bytesPerSecond);
		std::string returnInfo();
	};

	class Mute : public Converter {
	private:
		void countBorders(int& start_index, int& end_index, int stream_start, int stream_end, int buf_size, int elem_size);
	public:
		void convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int interval_start, int interval_end);
		void initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond);
		int calculateReadPos(const int readPos, int bytesPerSecond);
		std::string returnInfo();
	};

	class ChangeBeats : public Converter {
	private:
		void countBorders(int& start_index, int& end_index, int stream_start, int stream_end, int buf_size, int elem_size);
	public:
		void convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int interval_start, int interval_end);
		void initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond);
		int calculateReadPos(const int readPos, int bytesPerSecond);
		std::string returnInfo();
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

	class ChangeBeatsFactory : public ConverterFactory {
		Converter* factoryMethod();	
	};

	class SoundProcessor {
		std::vector<Converter*> converters;
		std::vector<wavfile::WavFile*> files;
		std::vector<unsigned short> input1;
		std::vector<unsigned short> input2;
		std::vector<unsigned short> output;
		int convFind(std::string& convToFind);
		void init_files(std::vector<std::vector<std::string>>& filenames);
		void init_converters(std::vector<std::vector<std::string>>& config);
	;	public:
		SoundProcessor();
		~SoundProcessor();
		void initialize(std::vector<std::vector<std::string>>& config, std::vector<std::vector<std::string>>& filenames);
		void run();
	};
}
