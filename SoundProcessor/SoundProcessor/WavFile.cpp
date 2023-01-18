#include "WavFile.h"
#include "Errors.h"
#include <fstream>

namespace wf = wavfile;
namespace es = errors;

////////////////////////////////////////////////////////private functions///////////////////////////////////////////////////

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
	static const unsigned long CHAR_SIZE = 256;
	static char tmp_str[sizeof(number)];
	for (int i = 1; i <= sizeof(number); i++) {
		tmp_str[sizeof(number) - i] = (char)(number % CHAR_SIZE);
		number /= CHAR_SIZE;
	}
	return tmp_str;
}

const char* wf::WavFile::num_str(unsigned short number) {
	static const unsigned long CHAR_SIZE = 256;
	static char tmp_str[sizeof(number)];
	for (int i = 1; i <= sizeof(number); i++) {
		tmp_str[sizeof(number) - i] = (char)(number % CHAR_SIZE);
		number /= CHAR_SIZE;
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
	//(*this).setDefaultHeader();
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
		//setDefaultHeader();
		open_status = CLOSED;
		throw errCode;
	}
}

void wf::WavFile::outInitialize(std::string filename) {
	fileStream.open(filename, std::ios_base::out | std::ios_base::binary);
	if (!(fileStream.is_open())) {
		throw es::CANNOT_OPEN_FILE;
	}
	//setDefaultHeader();
}

void wf::WavFile::readHeader() {
	const int NOT_OPEN = 0;
	const int BUF_SIZE = 1000;
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
	const int BUF_SIZE = 1000;
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
	int shortCount = (bytesCount + 1) / 2;
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
	int shortCount = (bytesCount + 1) / 2;
	//unsigned short test[5] = {1, 2, 3, 4, 5};
	std::memcpy(bufChar, data.data(), bytesCount);
	//for (int i = 0; i < shortCount; i++) {
	//	sprintf(bufChar + i * 2, "%hu", data[i]);	//maybe not work
	//}
	fileStream.seekg(writeIndex);
	fileStream.write(bufChar, shortCount * 2);
	delete[] bufChar;
	if (fileStream.bad()) throw es::WRITE_ERROR;
}

void wf::WavFile::writeData(std::vector<char>& data, int writeIndex, int bytesCount) {
	fileStream.seekg(writeIndex);
	fileStream.write(data.data(), bytesCount);
	if (fileStream.bad()) throw es::WRITE_ERROR;
}

void wf::WavFile::changeSize(unsigned long filesize) {
	chunkSize = filesize - 8;
	subchunk2Size = filesize - DEFAULT_HEADER_SIZE;
	fileSize = filesize;
}

int wf::WavFile::returnDatasize() {
	return subchunk2Size;
}

int wf::WavFile::isOpen() {
	return open_status;
}

//void wf::WavFile::setDefaultHeader() {
//	writeBytes(chunkId, wf::RIFF_CHAIN, 4);
//	chunkSize = DEFAULT_CHUNK_SIZE;	//data part is empty
//	writeBytes(format, wf::WAVE_FORMAT, 4);
//	writeBytes(subchunk1Id, wf::FMT_SUBCHAIN, 4);
//	subchunk1Size = wf::PCM_SUBCHUNK_SIZE;	//for PCM
//	audioFormat = wf::PCM_FORMAT;	//for PCM
//	numChannels = wf::DEFAULT_CHANNELS;
//	sampleRate = wf::DEFAULT_RATE;
//	bitsPerSample = wf::DEFAULT_BITS_PER_SAMPLE;
//	byteRate = wf::BYTES_PER_SECOND;	// 1 byte = 8 bits
//	blockAlign = DEFAULT_BLOCK_ALIGN;
//	writeBytes(subchunk2Id, wf::DATA_SUBCHAIN, 4);
//	subchunk2Size = DEFAULT_DATA_SIZE;	//data part is empty
//	fileSize = DEFAULT_HEADER_SIZE;
//	firstDataIndex = fileSize - subchunk2Size;
//	open_status = CLOSED;
//	headerSize = DEFAULT_HEADER_SIZE;
//	is_correct = wf::CORRECT;
//}

//void wf::WavFile::writeHeader() {
//	char out[DEFAULT_HEADER_SIZE];
//	writeBytes(out, chunkId, 4);
//	writeBytes(out + 4, num_str(chunkSize), 4);
//	writeBytes(out + 8, format, 4);
//	writeBytes(out + 12, subchunk1Id, 4);
//	writeBytes(out + 16, num_str(subchunk1Size), 4);
//	writeBytes(out + 20, num_str(audioFormat), 2);
//	writeBytes(out + 22, num_str(numChannels), 2);
//	writeBytes(out + 24, num_str(sampleRate), 4);
//	writeBytes(out + 28, num_str(byteRate), 4);
//	writeBytes(out + 32, num_str(blockAlign), 2);
//	writeBytes(out + 34, num_str(bitsPerSample), 2);
//	writeBytes(out + 36, subchunk2Id, 4);
//	writeBytes(out + 40, num_str(subchunk2Size), 4);
//	fileStream.seekg(0);
//	fileStream.write(out, DEFAULT_HEADER_SIZE);
//	if (fileStream.bad()) throw es::WRITE_ERROR;
//}

//int wf::WavFile::readData(std::vector<unsigned short>& data, int readIndex) {
//	if (open_status == CLOSED) return ZERO;
//	int bufSize = data.size() * 2;
//	std::vector<char> buf(bufSize);
//	static unsigned short buf_short[DEFAULT_BUF_SIZE];
//	int charToRead;
//	if (readIndex + bufSize >= fileSize) {
//		open_status = CLOSED;
//		charToRead = (fileSize - readIndex) % bufSize;
//	}
//	else {
//		charToRead = bufSize;
//	}
//	readIndex = readIndex + firstDataIndex;
//	fileStream.seekg(readIndex);
//	fileStream.read(buf.data(), charToRead);
//	if (fileStream.bad()) throw es::READ_ERROR;
//	std::memcpy(buf_short, buf.data(), charToRead);
//	data.clear();
//	data.insert(data.begin(), std::begin(buf_short), std::end(buf_short));
//	//data.insert(data.begin(), buf_short,  + bufSize);
//	return charToRead;
//}

//void wf::WavFile::writeHeader(char* header, int headerSize) {
//	fileStream.seekg(0);
//	fileStream.write(header, headerSize);
//	if (fileStream.bad()) throw es::WRITE_ERROR;
//}


