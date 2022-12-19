#pragma once
#include <vector>
#include <string>
#include "WavFile.h"

namespace converter {
	const std::string MIX_STR = "mix";
	const std::string MUTE_STR = "mute";
	const std::string LOWER_STR = "lower";
	enum conv_codes {
		MIX_CONVERTER = 0,
		MUTE_CONVERTER = 1,
		LOWER_CONVERTER = 2,
		NOT_CONVERTER = 3
	};

	class Converter {
	public:
		virtual void convert(std::vector<unsigned short>& input, std::vector<unsigned short>& output);
		virtual ~Converter() {}
	};

	class Mix : public Converter {
		void convert(std::vector<unsigned short>& input, std::vector<unsigned short>& output);
	};

	class Mute : public Converter {
		void convert(std::vector<unsigned short>& input, std::vector<unsigned short>& output);
	};

	class Lower : public Converter {
		void convert(std::vector<unsigned short>& input, std::vector<unsigned short>& output);
	};

	class ConverterFactory {
	public:
		virtual Converter* factoryMethod();
		std::vector<ConverterFactory*> initialize_array();
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
		std::vector<std::vector<int>> convertings;
		std::vector<unsigned short> sampleStream;
		std::vector<wavfile::WavFile*> files;
		int convFind(std::string convToFind, std::vector<std::string>& converterNames);
		std::vector<int> read_arguments(std::vector<std::string> source, int converterCode);
		void open_files(std::vector<std::string> files);
		void init_conertings();
		void init_converters();
	public:
		SoundProcessor();
		SoundProcessor(std::vector<std::vector<std::string>> configFile);
		~SoundProcessor();
		void run();
	};
}
