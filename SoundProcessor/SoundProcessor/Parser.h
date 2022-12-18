#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace parser {
	const int HEADER_SIZE = 40;
	const int DEFAULT_RATE = 44100;
	const int DEFAULT_CHANNELS = 1;
	const int DEFAULT_BITS_PER_SAMPLE = 16;
	const int PCM_SUBCHUNK_SIZE = 16;
	const int PCM_FORMAT = 1;
	const char RIFF_CHAIN[] = "RIFF";
	const char WAVE_FORMAT[] = "WAVE";
	const char FMT_SUBCHAIN[] = "fmt ";
	const char DATA_SUBCHAIN[] = "data";
	const bool CORRECT = 0;
	const bool INCORRECT = 1;
	const bool EQUAL = 0;
	const bool UNEQUAL = 1;
	const bool NO_DATA = 0;
	const bool DATA = 1;
	class Header {
		bool is_correct;
		char chunkId[4];				//for "RIFF" symbols
		unsigned long chunkSize;			//filesize - 8
		char format[4];					//for "WAVE" symbols
		char subchunk1Id[4];			//for "fmt " symbols
		unsigned long subchunk1Size;		//filesize - 16
		unsigned short audioFormat;			//1 for PCM - linear
		unsigned short numChannels;			//mono = 1
		unsigned long sampleRate;			//44100 Hz
		unsigned long byteRate;				// sampleRate * numChannels * bitsPerSample/8
		unsigned short blockAlign;			//numChannels * bitsPerSample/8   - bytes for 1 sample
		unsigned short bitsPerSample;		//sound depth
		char subchunk2Id[4];			//for "data symbols"
		unsigned long subchunk2Size;		//bites in data section
	public:
		Header();
		Header(std::string filename);
		~Header();
		void set_default_header();
		const bool iscorrect();
		const bool return_correctness();
		void copy_str(char* destination, const char* source, const int source_start, const int count);
		int compare_id(const char* id1, const char* id2);
		int readDataChunkId(char* array, int pos, int array_size);
	};
	const int BUF_SIZE = 4096;
	class data {
		std::vector<unsigned short> samples;
		int last_index;
	public:
		data();
		~data();
		int readSampleArray(std::ifstream& input);
	};
}