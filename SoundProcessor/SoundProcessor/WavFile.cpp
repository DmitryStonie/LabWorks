#include "WavFile.h"
#include "Errors.h"

namespace wf = wavfile;
namespace es = errors;

////////////////////////////////////////////////////////private functions///////////////////////////////////////////////////
void wf::WavFile::setDefaultHeader() {
	writeBytes(chunkId, wf::RIFF_CHAIN, 4);
	chunkSize = DEFAULT_CHUNK_SIZE;	//data part is empty
	writeBytes(format, wf::WAVE_FORMAT, 4);
	writeBytes(subchunk1Id, wf::FMT_SUBCHAIN, 4);
	subchunk1Size = wf::PCM_SUBCHUNK_SIZE;	//for PCM
	audioFormat = wf::PCM_FORMAT;	//for PCM
	numChannels = wf::DEFAULT_CHANNELS;
	sampleRate = wf::DEFAULT_RATE;
	bitsPerSample = wf::DEFAULT_BITS_PER_SAMPLE;
	byteRate = wf::BYTES_PER_SECOND;	// 1 byte = 8 bits
	blockAlign = DEFAULT_BLOCK_ALIGN;
	writeBytes(subchunk2Id, wf::DATA_SUBCHAIN, 4);
	subchunk2Size = DEFAULT_DATA_SIZE;	//data part is empty
	fileSize = DEFAULT_HEADER_SIZE;
	firstDataIndex = fileSize - subchunk2Size;
	open_status = CLOSED;
	headerSize = DEFAULT_HEADER_SIZE;
	is_correct = wf::CORRECT;
}

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

void wf::WavFile::copyStr(char* destination, const char* source, const int source_start, const int count) {
	for (int i = 0; i < count; i++) {
		destination[i] = source[i + source_start];
	}
}

int wf::WavFile::compareId(char* id1, const  char* id2) {
	for (int i = 0; i < CHUNK_NAME_SIZE; i++) {
		if (id1[i] != id2[i]) return UNEQUAL;
	}
	return EQUAL;
}

int wf::WavFile::readDataChunkId(char* array, int pos, int array_size) {
	while (pos != array_size - 1) {
		if (compareId(array + pos, wf::DATA_SUBCHAIN) == EQUAL) return pos + CHUNK_NAME_SIZE;
		pos++;
	}
	return NO_DATA;
}

const char* wf::WavFile::num_str(unsigned long number) {
	static char tmp_str[sizeof(number)];
	for (int i = 1; i <= sizeof(number); i++) {
		tmp_str[sizeof(number) - i] = (char)(number % sizeof(char));
		number /= sizeof(char);
	}
	return tmp_str;
}

const char* wf::WavFile::num_str(unsigned short number) {
	static char tmp_str[sizeof(number)];
	for (int i = 1; i <= sizeof(number); i++) {
		tmp_str[sizeof(number) - i] = (char)(number % sizeof(char));
		number /= sizeof(char);
	}
	return tmp_str;
}

void wf::WavFile::writeBytes(char* destination, const char* source, int source_size) {
	for (int i = 0; i < source_size; i++) {
		destination[i] = source[i];
	}
}

////////////////////////////////////////////////////////public functions///////////////////////////////////////////////////
wf::WavFile::WavFile() {
	(*this).setDefaultHeader();
}

wf::WavFile::~WavFile() {
	fileStream.close();
}

void wf::WavFile::initialize(std::string filename) {
	try {
		fileStream.open(filename, std::ios_base::in | std::ios_base::binary);
		if (!(fileStream.is_open())) {
			throw es::CANNOT_OPEN_FILE;
		}
		readHeader();
		open_status = OPENED;
	}
	catch (int errCode) {
		setDefaultHeader();
		open_status = CLOSED;
		throw errCode;
	}
}

void wf::WavFile::outInitialize(std::string filename) {
	fileStream.open(filename, std::ios_base::out | std::ios_base::binary);
	if (!(fileStream.is_open())) {
		throw es::CANNOT_OPEN_FILE;
	}
	setDefaultHeader();
}

void wf::WavFile::readHeader() {
	const int NOT_OPEN = 0;
	char buf_char[BUF_SIZE];
	int subchunk2SizeIndex;
	if (!(fileStream.read(buf_char, BUF_SIZE))) {
		throw es::CORRUPTED_HEADER;
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
			throw es::UNSUPPORTED_FILE_FORMAT;
		}
		copyStr(subchunk2Id, buf_char, subchunk2SizeIndex - 4, 4);
		std::memcpy(&subchunk2Size, buf_char + subchunk2SizeIndex, 4);
		fileSize = chunkSize + 8;
		headerSize = fileSize - subchunk2Size;
		firstDataIndex = fileSize - subchunk2Size;
		is_correct = (*this).isHeaderCorrect();
		if (is_correct == wf::INCORRECT) {
			throw es::UNSUPPORTED_FILE_FORMAT;
		}
	}
}

unsigned long wf::WavFile::returnHeaderSize() {
	return headerSize;
}

void wf::WavFile::returnHeader(char* buf_char) {
	fileStream.seekg(0);
	fileStream.read(buf_char, BUF_SIZE);
}

int wf::WavFile::readData(std::vector<unsigned short>& data, int readIndex, int bytesCount) {
	if (open_status == CLOSED || bytesCount == 0) return ZERO;
	int charToRead;
	if (readIndex + bytesCount >= fileSize) {
		open_status = CLOSED;
		charToRead = (fileSize - readIndex) % bytesCount;
	}
	else {
		charToRead = bytesCount;
	}
	int shortCount = (bytesCount + 1) / sizeof(unsigned short);
	unsigned short* bufShort = new unsigned short[shortCount];
	char* bufChar = new char[charToRead];
	fileStream.seekg(readIndex);
	fileStream.read(bufChar, charToRead);
	if (fileStream.bad()) {
		delete[] bufShort;
		delete[] bufChar;
		throw es::READ_ERROR;
	}
	std::memcpy(bufShort, bufChar, charToRead);
	data.clear();
	data.insert(data.begin(), bufShort, bufShort + shortCount);
	delete[] bufChar;
	delete[] bufShort;
	return charToRead;
}

int wf::WavFile::readData(std::vector<char>& data, int readIndex, int bytesCount) {
	if (open_status == CLOSED || bytesCount == 0) return ZERO;
	int charToRead;
	if (readIndex + bytesCount >= fileSize) {
		open_status = CLOSED;
		charToRead = (fileSize - readIndex) % bytesCount;
	}
	else {
		charToRead = bytesCount;
	}
	char* bufChar = new char[charToRead];
	fileStream.seekg(readIndex);
	fileStream.read(bufChar, charToRead);
	if (fileStream.bad()) {
		delete[] bufChar;
		throw es::READ_ERROR;
	}
	data.clear();
	data.insert(data.begin(), bufChar, bufChar + charToRead);
	delete[] bufChar;
	return charToRead;
}

void wf::WavFile::writeData(std::vector<unsigned short>& data, int writeIndex, int bytesCount) {
	char* bufChar = new char[bytesCount];
	int shortCount = (bytesCount + 1) / sizeof(unsigned short);
	std::memcpy(bufChar, data.data(), bytesCount);
	fileStream.seekg(writeIndex);
	fileStream.write(bufChar, shortCount * sizeof(unsigned short));
	delete[] bufChar;
	if (fileStream.bad()) throw es::WRITE_ERROR;
}

void wf::WavFile::writeData(std::vector<char>& data, int writeIndex, int bytesCount) {
	fileStream.seekg(writeIndex);
	fileStream.write(data.data(), bytesCount);
	if (fileStream.bad()) throw es::WRITE_ERROR;
}

void wf::WavFile::changeSize(unsigned long filesize) {
	chunkSize = filesize - RIFF_NAME_SIZE;
	subchunk2Size = filesize - DEFAULT_HEADER_SIZE;
	fileSize = filesize;
}

int wf::WavFile::returnDatasize() {
	return subchunk2Size;
}

int wf::WavFile::isOpen() {
	return open_status;
}
