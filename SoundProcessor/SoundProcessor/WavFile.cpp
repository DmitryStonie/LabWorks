#include "WavFile.h"
#include <fstream>

namespace wf = wavfile;

wf::WavFile::WavFile() {
	(*this).setDefaultHeader();
}

wf::WavFile::~WavFile() {
	fileStream.close();
}

void wf::WavFile::initialize(std::string filename) {
	fileStream.open(filename, std::ios_base::in | std::ios_base::binary);
	if (!(fileStream.is_open())) {
		setDefaultHeader();
		open_status = CLOSED;
		throw CANNOT_OPEN_FILE;
	}
	bool header_status = readHeader();
	if(header_status == NOT_SUCCESFUL) {
		open_status = CLOSED;
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
	fileSize = DEFAULT_HEADER_SIZE;
	firstDataIndex = fileSize - subchunk2Size;
}

int wf::WavFile::isOpen() {
	return open_status;
}

void wf::WavFile::changeSize(unsigned long filesize) {
	chunkSize = filesize - 8;
	subchunk1Size = PCM_SUBCHUNK_SIZE;
	subchunk2Size = filesize - DEFAULT_HEADER_SIZE;
	fileSize = filesize;
}

bool wf::WavFile::readHeader() {
	const int NOT_OPEN = 0;
	const int BUF_SIZE = 1000;
	char buf_char[BUF_SIZE];
	int subchunk2SizeIndex;
	if (!(fileStream.read(buf_char, BUF_SIZE))) {
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
		fileSize = chunkSize + 8;
		firstDataIndex = fileSize - subchunk2Size;
		is_correct = (*this).isHeaderCorrect();
		if (is_correct == wf::INCORRECT) {
			std::cerr << "Header is not a WAV header. Can't use this file\n";
			return NOT_SUCCESFUL;
		}
	}
	return SUCCESFUL;
}

void wf::WavFile::writeHeader() {
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
	fileStream.write(out, DEFAULT_HEADER_SIZE);
	fileSize += DEFAULT_HEADER_SIZE;
	firstDataIndex = DEFAULT_HEADER_SIZE;

}

int wf::WavFile::readData(std::vector<unsigned short> data, int readIndex) {
	if (open_status == CLOSED) return ZERO;
	int bufSize = data.size() * 2;
	std::vector<char> buf(bufSize);
	static unsigned short* buf_short[DEFAULT_BUF_SIZE];
	int charToRead;
	if (readIndex + bufSize >= fileSize) {
		open_status = CLOSED;
		charToRead = (fileSize - readIndex) % bufSize;
	}
	else {
		charToRead = bufSize;
	}
	readIndex = readIndex + firstDataIndex;
	fileStream.seekg(readIndex);
	fileStream.read(buf.data(), charToRead);
	std::memcpy(buf_short, buf.data(), charToRead);
	data.insert(data.end(), buf_short, buf_short + bufSize);
	return charToRead;
}

void wf::WavFile::writeData(std::vector<unsigned short> data, int writeIndex) {
	int bufSize = data.size() * 2;
	std::vector<char> buf(bufSize);
	for (int i = 0; i < DEFAULT_BUF_SIZE; i++) {
		sprintf(buf.data() + i * 2, "%hu", data[i]);
	}
	fileStream.seekg(writeIndex);
	fileStream.write(buf.data(), DEFAULT_BUF_SIZE * 2);
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

unsigned long wf::WavFile::returnDataPos() {
	return fileSize - subchunk2Size;
}

void wf::WavFile::outInitialize(std::string filename) {
	fileStream.open(filename, std::ios_base::out | std::ios_base::binary);
	if (!(fileStream.is_open())) {
		throw CANNOT_OPEN_FILE;
	}
	setDefaultHeader();
}