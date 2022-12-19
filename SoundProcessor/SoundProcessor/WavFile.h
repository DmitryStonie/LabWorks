#pragma once
#include <iostream>
#include <string>
#include <vector>

namespace wavfile {
	const int HEADER_SIZE = 40;
	const int DEFAULT_HEADER_SIZE = 44;
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
	const int DEFAULT_BUF_SIZE = 4096;
	const bool SUCCESFUL = 0;
	const bool NOT_SUCCESFUL = 1;
	const int ERROR = 123;
	const int DEFAULT_LAST_INDEX = 0;
	const bool OPENED = 0;
	const bool CLOSED = 1;
	enum exceptions {
		CANNOT_OPEN_FILE = 0,
		UNSUPPORTED_HEADER = 1
	};
	class WavFile {
		//header
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
		//data
		std::vector<unsigned short> samples;
		int last_index;
		int open_status;

		const bool isHeaderCorrect();
		const bool returnHeaderCorrectness();
		void copyStr(char* destination, const char* source, const int source_start, const int count);
		int compareId(const char* id1, const char* id2);
		int readDataChunkId(char* array, int pos, int array_size);
		const char* num_str(unsigned long number);
		const char* num_str(unsigned short number);
	public:
		WavFile();
		~WavFile();
		void initialize(std::string filename);
		void setDefaultHeader();
		void changeSize(unsigned long filesize);
		int isOpen();
		
		bool readHeader(std::fstream& input);
		void writeHeader(std::fstream& output);
		bool readData(std::fstream& input);
		void writeData(std::fstream& output);
	};

}