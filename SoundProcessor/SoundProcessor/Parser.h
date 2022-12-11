#pragma once
#include <string>

namespace parser {
	const int HEADER_SIZE = 44;
	class Header {
		char chunkId[4];					//for "RIFF" symbols
		long long chunkSize;				//filesize - 8
		char format[4];						//for "WAVE" symbols
		char subchunk1Id[4];				//for "fmt " symbols
		unsigned long subchunk1Size;		//filesize - 16
		short int audioFormat;				//1 for PCM - linear
		short int numChannels;				//mono = 1
		unsigned long sampleRate;			//44100 Hz
		unsigned long byteRate;				// sampleRate * numChannels * bitsPerSample/8
		short int blockAlign;				//numChannels * bitsPerSample/8   - bytes for 1 sample
		short int bitsPerSample;			//sound depth
		char subchunk2Id[4];				//for "data symbols"
		unsigned long sunchunk2Size;		//bites in data section
	public:
		Header();
		Header(std::string filename);
		~Header();
	};
}