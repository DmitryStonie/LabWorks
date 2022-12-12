#include "Parser.h"
#include <iostream>
#include <cstdio>
#include <string>

parser::Header::Header() {
	(*this).set_default_header();
}

parser::Header::Header(std::string filename) {
	(*this).set_default_header();
	FILE* input_file = fopen( "severe_tire_damage.wav", "rb");
	if (input_file == NULL) {
		std::cerr << "Unable to open file, error \n";
		return;
	}
	//default
	char buf_char[HEADER_SIZE];
	std::string buffer, buffer2;
	int read_elem = 0;
	read_elem = fread(buf_char, 1, HEADER_SIZE, input_file);
	buffer = read_elem;
	if (sizeof(buffer) != HEADER_SIZE) {
		std::cerr << "Header is corrupted.\n";
	}
	else {
		chunkId = buffer.substr(0, 4);
		buffer2 = buffer.substr(4, 4);
		chunkSize = stoi(buffer2);
		format = buffer.substr(8, 4);
		subchunk1Id = buffer.substr(12, 4);
		buffer2 = buffer.substr(16, 4);
		subchunk1Size = stoi(buffer2);
		buffer2 = buffer.substr(20, 2);
		audioFormat = stoi(buffer2);
		buffer2 = buffer.substr(22, 2);
		numChannels = stoi(buffer2);
		buffer2 = buffer.substr(24, 4);
		sampleRate = stoi(buffer2);
		buffer2 = buffer.substr(28, 4);
		byteRate = stoi(buffer2);
		buffer2 = buffer.substr(32, 2);
		blockAlign = stoi(buffer2);
		buffer2 = buffer.substr(34, 2);
		bitsPerSample = stoi(buffer2);
		subchunk2Id = buffer.substr(36, 4);
		buffer2 = buffer.substr(40, 4);
		subchunk2Size = stoi(buffer2);
		is_correct = (*this).iscorrect();
		if (is_correct == INCORRECT) {
			std::cerr << "Header is not a WAV header. Can't use this file\n";
		}
	}
}

parser::Header::~Header() {

}

void parser::Header::set_default_header() {
	is_correct = CORRECT;
	chunkId = RIFF_CHAIN;
	chunkSize = HEADER_SIZE - 4;	//data part is empty	
	format = WAVE_FORMAT;
	subchunk1Id = FMT_SUBCHAIN;
	subchunk1Size = PCM_SUBCHUNK_SIZE;	//for PCM
	audioFormat = PCM_FORMAT;	//for PCM
	numChannels = DEFAULT_CHANNELS;
	sampleRate = DEFAULT_RATE;
	bitsPerSample = DEFAULT_BITS_PER_SAMPLE;
	byteRate = sampleRate * numChannels * bitsPerSample / 8;	// 1 byte = 8 bits
	blockAlign = numChannels * bitsPerSample / 8;
	subchunk2Id = DATA_SUBCHAIN;
	subchunk2Size = 0;	//data part is empty
}

const bool parser::Header::iscorrect() {
	if (chunkId == RIFF_CHAIN && format == WAVE_FORMAT && subchunk1Id == FMT_SUBCHAIN
		&& subchunk2Id == DATA_SUBCHAIN && sampleRate == DEFAULT_RATE
		&& bitsPerSample == DEFAULT_BITS_PER_SAMPLE && numChannels == DEFAULT_CHANNELS
		&& audioFormat == PCM_FORMAT && subchunk1Size == PCM_SUBCHUNK_SIZE) {
		return CORRECT;
	}
	else {
		return INCORRECT;
	}
}

const bool parser::Header::return_correctness() {
	return is_correct;
}