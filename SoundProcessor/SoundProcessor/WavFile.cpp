#include "WavFile.h"
#include <fstream>

namespace wf = wavfile;

wf::WavFile::WavFile() {
	(*this).setDefaultHeader();
}

wf::WavFile::~WavFile() {

}

void wf::WavFile::initialize(std::string filename) {
	std::fstream filestream(filename);
	if (!(filestream.is_open())) {
		setDefaultHeader();
		open_status = CLOSED;
		throw CANNOT_OPEN_FILE;
	}
	bool header_status = readHeader(filestream);
	if(header_status == NOT_SUCCESFUL) {
		throw UNSUPPORTED_HEADER;
		setDefaultHeader();
	}
	open_status = OPENED;
}

void wf::WavFile::setDefaultHeader() {
	is_correct = wf::CORRECT;
	copyStr(chunkId, wf::RIFF_CHAIN, 0, 4);
	chunkSize = wf::DEFAULT_HEADER_SIZE - 4;	//data part is empty
	copyStr(format, wf::WAVE_FORMAT, 8, 4);
	copyStr(subchunk1Id, wf::FMT_SUBCHAIN, 12, 4);
	subchunk1Size = wf::PCM_SUBCHUNK_SIZE;	//for PCM
	audioFormat = wf::PCM_FORMAT;	//for PCM
	numChannels = wf::DEFAULT_CHANNELS;
	sampleRate = wf::DEFAULT_RATE;
	bitsPerSample = wf::DEFAULT_BITS_PER_SAMPLE;
	byteRate = sampleRate * numChannels * bitsPerSample / 8;	// 1 byte = 8 bits
	blockAlign = numChannels * bitsPerSample / 8;
	copyStr(subchunk2Id, wf::DATA_SUBCHAIN, 36, 4);
	subchunk2Size = 0;	//data part is empty
}

int wf::WavFile::isOpen() {
	return open_status;
}

void wf::WavFile::changeSize(unsigned long filesize) {
	chunkSize = filesize - 8;
	subchunk1Size = PCM_SUBCHUNK_SIZE;
	subchunk2Size = filesize - DEFAULT_HEADER_SIZE;
}

bool wf::WavFile::readHeader(std::fstream& input) {
	const int NOT_OPEN = 0;
	const int BUF_SIZE = 1000;
	char buf_char[BUF_SIZE];
	int subchunk2SizeIndex;
	if (input.read(buf_char, BUF_SIZE)) {
		std::cerr << "Header is corrupted.\n";
		return NOT_SUCCESFUL;
	}
	else {
		copyStr(chunkId, buf_char, 0, 4);
		std::memcpy(&chunkSize, buf_char + 4, 4);
		copyStr(format, buf_char, 8, 4);
		copyStr(subchunk1Id, buf_char, 12, 4);
		std::memcpy(&subchunk1Size, buf_char + 16, 4);
		std::memcpy(&audioFormat, buf_char + 20, 2);
		std::memcpy(&numChannels, buf_char + 22, 2);
		std::memcpy(&sampleRate, buf_char + 24, 4);
		std::memcpy(&byteRate, buf_char + 28, 4);
		std::memcpy(&blockAlign, buf_char + 32, 2);
		std::memcpy(&bitsPerSample, buf_char + 34, 2);
		copyStr(subchunk2Id, buf_char, 36, 4);
		subchunk2SizeIndex = this->readDataChunkId(buf_char, DEFAULT_HEADER_SIZE - sizeof(subchunk2Size), BUF_SIZE);
		if (subchunk2SizeIndex == 0) {
			std::cerr << "Header is not a WAV header. Can't use this file\n";
			return NOT_SUCCESFUL;
		}
		copyStr(subchunk2Id, buf_char, subchunk2SizeIndex - 4, 4);
		std::memcpy(&subchunk2Size, buf_char + subchunk2SizeIndex, 4);
		is_correct = (*this).isHeaderCorrect();
		if (is_correct == wf::INCORRECT) {
			std::cerr << "Header is not a WAV header. Can't use this file\n";
			return NOT_SUCCESFUL;
		}
		else {
			input.seekg(subchunk2SizeIndex + 4);
			input.read(buf_char, 4);
		}
	}
	return SUCCESFUL;
}

void wf::WavFile::writeHeader(std::fstream& output) {
	char out[DEFAULT_HEADER_SIZE];
	strcat(out, chunkId);
	strcat(out, num_str(chunkSize));
	strcat(out, format);
	strcat(out, subchunk1Id);
	strcat(out, num_str(subchunk1Size));
	strcat(out, num_str(audioFormat));
	strcat(out, num_str(numChannels));
	strcat(out, num_str(sampleRate));
	strcat(out, num_str(byteRate));
	strcat(out, num_str(blockAlign));
	strcat(out, num_str(bitsPerSample));
	strcat(out, subchunk2Id);
	strcat(out, num_str(subchunk2Size));
	output.write(out, DEFAULT_HEADER_SIZE);
}

bool wf::WavFile::readData(std::fstream& input) {
	static char buf[DEFAULT_BUF_SIZE * 2];
	input.read(buf, DEFAULT_BUF_SIZE * 2);
	std::memcpy(&samples, buf, DEFAULT_BUF_SIZE * 2);
	return SUCCESFUL;
}

void wf::WavFile::writeData(std::fstream& output) {
	static char buf[DEFAULT_BUF_SIZE * 2];
	for (int i = 0; i < DEFAULT_BUF_SIZE; i++) {
		sprintf(buf + i * 2, "%hu", samples[i]);
	}
	output.write(buf, DEFAULT_BUF_SIZE * 2);
};

const bool wf::WavFile::isHeaderCorrect() {
	if (compareId(chunkId, wf::RIFF_CHAIN) == wf::EQUAL && compareId(format, wf::WAVE_FORMAT) == wf::EQUAL
		&& compareId(subchunk1Id, wf::FMT_SUBCHAIN) == wf::EQUAL && compareId(subchunk2Id, wf::DATA_SUBCHAIN) == wf::EQUAL
		&& sampleRate == wf::DEFAULT_RATE && bitsPerSample == wf::DEFAULT_BITS_PER_SAMPLE
		&& numChannels == wf::DEFAULT_CHANNELS && audioFormat == wf::PCM_FORMAT && subchunk1Size == wf::PCM_SUBCHUNK_SIZE) {
		return wf::CORRECT;
	}
	else {
		return wf::INCORRECT;
	}
}

const bool wf::WavFile::returnHeaderCorrectness() {
	return is_correct;
}

void wf::WavFile::copyStr( char* destination, const char* source, const int source_start, const int count) {
	for (int i = 0; i < count; i++) {
		destination[i] = source[i + source_start];
	}
}

int wf::WavFile::compareId(const  char* id1, const  char* id2) {
	for (int i = 0; i < 4; i++) {
		if (id1[i] != id2[i]) return UNEQUAL;
	}
	return EQUAL;
}

int wf::WavFile::readDataChunkId(char* array, int pos, int array_size) {
	const int chunkNameSize = 4;
	while (pos != array_size - 1) {
		if (compareId(array + pos, wf::DATA_SUBCHAIN) == EQUAL) return pos + chunkNameSize;
		pos++;
	}
	return NO_DATA;
}

const char* wf::WavFile::num_str(unsigned long number) {
	static std::string tmp_str;
	tmp_str = std::to_string(number);
	tmp_str = tmp_str.substr(0, 4);
	return tmp_str.data();
}

const char* wf::WavFile::num_str(unsigned short number) {
	static std::string tmp_str;
	tmp_str = std::to_string(number);
	tmp_str = tmp_str.substr(0, 4);
	return tmp_str.data();
}

