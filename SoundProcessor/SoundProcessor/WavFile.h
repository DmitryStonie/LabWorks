#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>

namespace wavfile {
	const char RIFF_CHAIN[4] = { 'R', 'I', 'F', 'F' };
	const int DEFAULT_CHUNK_SIZE = 36;
	const char WAVE_FORMAT[4] = { 'W', 'A', 'V', 'E' };
	const char FMT_SUBCHAIN[4] = { 'f', 'm', 't', ' ' };
	const int PCM_SUBCHUNK_SIZE = 16;
	const int PCM_FORMAT = 1;
	const int DEFAULT_CHANNELS = 1;
	const int DEFAULT_RATE = 44100;
	const int BYTES_PER_SECOND = 88200;
	const int DEFAULT_BLOCK_ALIGN = 2;
	const int DEFAULT_BITS_PER_SAMPLE = 16;
	const char DATA_SUBCHAIN[4] = { 'd', 'a', 't', 'a' };
	const int DEFAULT_DATA_SIZE = 0;


	const int DEFAULT_HEADER_SIZE = 44;
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
	const int ZERO = 0;
	class WavFile {
		//header
		char chunkId[4];				//for "RIFF" symbols
		unsigned long chunkSize;		//filesize - 8
		char format[4];					//for "WAVE" symbols
		char subchunk1Id[4];			//for "fmt " symbols
		unsigned long subchunk1Size;	//filesize - 16
		unsigned short audioFormat;		//1 for PCM - linear
		unsigned short numChannels;		//mono = 1
		unsigned long sampleRate;		//44100 Hz
		unsigned long byteRate;			// sampleRate * numChannels * bitsPerSample/8
		unsigned short blockAlign;		//numChannels * bitsPerSample/8   - bytes for 1 sample
		unsigned short bitsPerSample;	//sound depth
		char subchunk2Id[4];			//for "data symbols"
		unsigned long subchunk2Size;	//bites in data section
		//data
		int open_status;
		int firstDataIndex;
		unsigned long fileSize;
		unsigned long headerSize;
		std::fstream fileStream;
		bool is_correct;

		const bool isHeaderCorrect();
		const bool returnHeaderCorrectness();
		void copyStr(char* destination, const char* source, const int source_start, const int count);
		int compareId(char* id1, const  char* id2);
		int readDataChunkId(char* array, int pos, int array_size);
		const char* num_str(unsigned long number);
		const char* num_str(unsigned short number);
		void writeBytes(char* destination, const char* source, int source_size);
	public:
		WavFile();
		~WavFile();
		void initialize(std::string filename);
		void changeSize(unsigned long filesize);
		int returnDatasize();
		int isOpen();
		void outInitialize(std::string filename);
		void setDefaultHeader();
		unsigned long returnHeadersize();
		void returnHeader(char* buf_char);

		void readHeader();
		void writeHeader();
		void writeHeader(char* header, int headerSize);
		int readData(std::vector<unsigned short> data, int readIndex);
		void writeData(std::vector<unsigned short> data, int writeIndex, int bytesCount);
	};

}