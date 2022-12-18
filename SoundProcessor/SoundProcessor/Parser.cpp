#include "Parser.h"
#include <fstream>

namespace ps = parser;

ps::Header::Header() {
	(*this).set_default_header();
}
ps::Header::Header(std::string filename) {
	const int NOT_OPEN = 0;
	const int BUF_SIZE = 1000;
	(*this).set_default_header();
	std::ifstream input_file("severe_tire_damage.wav");
	if (!input_file.is_open()) {
		std::cerr << "Unable to open file, error \n";
		(*this).set_default_header();
		return;
	}
	//default
	char buf_char[BUF_SIZE];
	int subchunk2SizeIndex;
	if (!input_file.read(buf_char, BUF_SIZE)) {
		std::cerr << "Header is corrupted.\n";
	}
	else {
		copy_str(chunkId, buf_char, 0, 4);
		std::memcpy(&chunkSize, buf_char + 4, 4);
		copy_str(format, buf_char, 8, 4);
		copy_str(subchunk1Id, buf_char, 12, 4);
		std::memcpy(&subchunk1Size, buf_char + 16, 4);
		std::memcpy(&audioFormat, buf_char + 20, 2);
		std::memcpy(&numChannels, buf_char + 22, 2);
		std::memcpy(&sampleRate, buf_char + 24, 4);
		std::memcpy(&byteRate, buf_char + 28, 4);
		std::memcpy(&blockAlign, buf_char + 32, 2);
		std::memcpy(&bitsPerSample, buf_char + 34, 2);
		copy_str(subchunk2Id, buf_char, 36, 4);
		subchunk2SizeIndex = this->readDataChunkId(buf_char, HEADER_SIZE, BUF_SIZE);
		if (subchunk2SizeIndex == 0) {
			std::cerr << "Header is not a WAV header. Can't use this file\n";
			return;

		}
		copy_str(subchunk2Id, buf_char, subchunk2SizeIndex - 4, 4);
		std::memcpy(&subchunk2Size, buf_char + subchunk2SizeIndex, 4);
		is_correct = (*this).iscorrect();
		if (is_correct == ps::INCORRECT) {
			std::cerr << "Header is not a WAV header. Can't use this file\n";
		}
		else {
			input_file.seekg(subchunk2SizeIndex+4);
			input_file.read(buf_char, 4);
		}
	}
}

ps::Header::~Header() {

}

void ps::Header::set_default_header() {
	is_correct = ps::CORRECT;
	copy_str(chunkId, ps::RIFF_CHAIN, 0, 4);
	chunkSize = ps::HEADER_SIZE - 4;	//data part is empty
	copy_str(format, ps::WAVE_FORMAT, 8, 4);
	copy_str(subchunk1Id, ps::FMT_SUBCHAIN, 12, 4);
	subchunk1Size = ps::PCM_SUBCHUNK_SIZE;	//for PCM
	audioFormat = ps::PCM_FORMAT;	//for PCM
	numChannels = ps::DEFAULT_CHANNELS;
	sampleRate = ps::DEFAULT_RATE;
	bitsPerSample = ps::DEFAULT_BITS_PER_SAMPLE;
	byteRate = sampleRate * numChannels * bitsPerSample / 8;	// 1 byte = 8 bits
	blockAlign = numChannels * bitsPerSample / 8;
	copy_str(subchunk2Id, ps::DATA_SUBCHAIN, 36, 4);
	subchunk2Size = 0;	//data part is empty
}

const bool ps::Header::iscorrect() {
	if (compare_id(chunkId, ps::RIFF_CHAIN) == ps::EQUAL && compare_id(format, ps::WAVE_FORMAT) == ps::EQUAL
		&& compare_id(subchunk1Id, ps::FMT_SUBCHAIN) == ps::EQUAL && compare_id(subchunk2Id, ps::DATA_SUBCHAIN) == ps::EQUAL
		&& sampleRate == ps::DEFAULT_RATE && bitsPerSample == ps::DEFAULT_BITS_PER_SAMPLE
		&& numChannels == ps::DEFAULT_CHANNELS && audioFormat == ps::PCM_FORMAT && subchunk1Size == ps::PCM_SUBCHUNK_SIZE) {
		return ps::CORRECT;
	}
	else {
		return ps::INCORRECT;
	}
}

const bool ps::Header::return_correctness() {
	return is_correct;
}

void ps::Header::copy_str( char* destination, const char* source, const int source_start, const int count) {
	for (int i = 0; i < count; i++) {
		destination[i] = source[i + source_start];
	}
}

int ps::Header::compare_id(const  char* id1, const  char* id2) {
	for (int i = 0; i < 4; i++) {
		if (id1[i] != id2[i]) return ps::UNEQUAL;
	}
	return ps::EQUAL;
}

int ps::Header::readDataChunkId(char* array, int pos, int array_size) {
	const int chunkNameSize = 4;
	while (pos != array_size - 1) {
		if (compare_id(array + pos, ps::DATA_SUBCHAIN) == EQUAL) return pos + chunkNameSize;
		pos++;
	}
	return NO_DATA;
}

ps::data::data() {

}

ps::data::~data() {

}

int ps::data::readSampleArray(std::ifstream& input, int arraySize) {
	char buf[BUF_SIZE * 2];
	if (!(input.read(buf, BUF_SIZE * 2))) {
		return ERROR;
	}
	std::memcpy(&samples, buf, BUF_SIZE*2);
}