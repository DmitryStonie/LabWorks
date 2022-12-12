#pragma once
#include <string>

namespace parser {
	const int HEADER_SIZE = 44;
	const int DEFAULT_RATE = 44100;
	const int DEFAULT_CHANNELS = 1;
	const int DEFAULT_BITS_PER_SAMPLE = 16;
	const int PCM_SUBCHUNK_SIZE = 16;
	const int PCM_FORMAT = 1;
	const std::string RIFF_CHAIN = "RIFF";
	const std::string WAVE_FORMAT = "WAVE";
	const std::string FMT_SUBCHAIN = "fmt ";
	const std::string DATA_SUBCHAIN = "data";
	const bool CORRECT = 0;
	const bool INCORRECT = 1;
	class Header {
		bool is_correct;
		std::string chunkId;				//for "RIFF" symbols
		unsigned long chunkSize;			//filesize - 8
		std::string format;					//for "WAVE" symbols
		std::string subchunk1Id;			//for "fmt " symbols
		unsigned long subchunk1Size;		//filesize - 16
		unsigned short audioFormat;			//1 for PCM - linear
		unsigned short numChannels;			//mono = 1
		unsigned long sampleRate;			//44100 Hz
		unsigned long byteRate;				// sampleRate * numChannels * bitsPerSample/8
		unsigned short blockAlign;			//numChannels * bitsPerSample/8   - bytes for 1 sample
		unsigned short bitsPerSample;		//sound depth
		std::string subchunk2Id;			//for "data symbols"
		unsigned long subchunk2Size;		//bites in data section
	public:
		Header();
		Header(std::string filename);
		~Header();
		void set_default_header();
		const bool iscorrect();
		const bool return_correctness();
	};
}