#include "Converters.h"
#include "WavFile.h"
#include "Errors.h"
#include "ArgsContainer.h"
#include <limits>
#include <algorithm>

namespace cv = converter;
namespace wf = wavfile;
namespace ac = argscontainer;

void cv::Mix::convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int stream_start, int stream_end) {
	int start_index = 0, end_index = 0, output_value = 0;
	const int MIX_PADDING = 2;
	countBorders(start_index, end_index, stream_start, stream_end, output.size(), sizeof(unsigned short));
	if (start_index < end_index) {
		for (int i = start_index; i < end_index; i += MIX_PADDING) {
			output[i] = input1[i];
		}
		for (int i = start_index + 1; i < end_index; i += MIX_PADDING) {
			output[i] = input2[i];
		}
	}
	else {
		input1.swap(output);
	}
}

void cv::Mix::initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond) {
	if (parameters.size() < PARAMETERS_NUM) {
		interval_start = 0;
		interval_end = 0;
		return;
	}
	interval_start = atoi(parameters[2].data()) * bytesPerSecond;
	interval_end = MAX_DATA_SIZE;
	secondStreamNumber = atoi((parameters[1].substr(1)).data());
}

int cv::Mix::calculateReadPos(const int readPos, int bytesPerSecond) {
	return readPos;
}

void cv::Mix::countBorders(int& start_index, int& end_index, int stream_start, int stream_end, int buf_size, int elem_size) {
	if (stream_start <= interval_start) start_index = 0;
	else start_index = stream_start;
	if (stream_end <= interval_start) end_index = 0;
	else end_index = stream_end;
	start_index = (start_index / elem_size) % buf_size;
	if(end_index != 0) end_index = (end_index / elem_size) % buf_size ? (end_index / elem_size) % buf_size : buf_size;
}

std::string cv::Mix::returnInfo() {
	return "Converter name: Mixer\nfunction: mixes two songs into 1 at the same place in both songs\nparamenets:\n\tmix - name of converter\n\tnumber_of_second_file - number of file to mix with first file. 1 means mix with same file, 2 - with next opened input file\n\tfrom_second - number of second from which mix starts up to the end of the file\nsyntax: mix $<number_of_second_file> <from_second>\n\n";
}

void cv::Mute::convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int stream_start, int stream_end) {
	int start_index = 0, end_index = 0;
	countBorders(start_index, end_index, stream_start, stream_end, output.size(), sizeof(unsigned short));
	if (start_index < end_index) {
		std::fill(output.begin() + start_index, output.begin() + end_index, ZERO);
	}
	else {
		input1.swap(output);
	}
}

void cv::Mute::initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond) {
	if (parameters.size() < PARAMETERS_NUM) {
		interval_start = 0;
		interval_end = 0;
		return;
	}
	interval_start = atoi(parameters[1].data()) * bytesPerSecond;
	interval_end = atoi(parameters[2].data()) * bytesPerSecond;
	secondStreamNumber = NO_SECOND_STREAM;
}

int cv::Mute::calculateReadPos(const int readPos, int bytesPerSecond) {
	return readPos;
}

void cv::Mute::countBorders(int& start_index, int& end_index, int stream_start, int stream_end, int buf_size, int elem_size) {
	if (stream_start <= interval_start) start_index = 0;
	else if (stream_start <= interval_end) start_index = stream_start;
	else start_index = interval_end;
	if (stream_end <= interval_start) end_index = 0;
	else if (stream_end <= interval_end) end_index = stream_end;
	else end_index = interval_end;
	start_index = (start_index / elem_size) % buf_size;
	if (end_index != 0) end_index = (end_index / elem_size) % buf_size ? (end_index / elem_size) % buf_size : buf_size;
}

std::string cv::Mute::returnInfo() {
	return "Converter name: Muter\nfunction: make silense in period of the song\nparamenets:\n\tmute - name of converter\n\tfrom_second - number of second from which mute works\n\tto_second - number of second from which mute works\nsyntax: mute <from_second> <to_second>\n\n";
}

void cv::ChangeBeats::convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int stream_start, int stream_end) {
	const int BEATS_IN_SECOND = 2;
	int start_index = 0, end_index = 0, output_value = 0, input_index = 0;
	countBorders(start_index, end_index, stream_start, stream_end, output.size(), sizeof(unsigned short));
	input_index = stream_start;
	int index = start_index, numOfSecond = stream_start / wf::BYTES_PER_SECOND;
	int firstBitBorder = interval_start + wf::BYTES_PER_SECOND * numOfSecond + wf::BYTES_PER_SECOND / BEATS_IN_SECOND;
	int secondBitBorder = interval_start + wf::BYTES_PER_SECOND * numOfSecond + wf::BYTES_PER_SECOND;
	int thirdBitBorder = interval_start + wf::BYTES_PER_SECOND * numOfSecond + (wf::BYTES_PER_SECOND / BEATS_IN_SECOND + wf::BYTES_PER_SECOND);
	if (stream_end >= interval_end) {
		secondStreamNumber = NO_SECOND_STREAM;
		input1.swap(output);
		return;
	}
	if(start_index >= end_index) {
		input1.swap(output);
		return;
	}
	while (index < end_index){
		for (; index < end_index && (input_index <= firstBitBorder || (input_index > secondBitBorder && input_index <= thirdBitBorder)); index++, input_index++) {
			output[index] = input1[index];
		}
		for (; index < end_index && ((input_index > firstBitBorder && input_index <= secondBitBorder) || input_index > thirdBitBorder); index++, input_index++) {
			output[index] = input2[index];
		}
		firstBitBorder += wf::BYTES_PER_SECOND * BEATS_IN_SECOND;
		secondBitBorder += wf::BYTES_PER_SECOND * BEATS_IN_SECOND;
		thirdBitBorder += wf::BYTES_PER_SECOND * BEATS_IN_SECOND;
	}
}

void cv::ChangeBeats::initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond) {
	if (parameters.size() < PARAMETERS_NUM) {
		interval_start = 0;
		interval_end = 0;
		return;
	}
	interval_start = atoi(parameters[1].data()) * bytesPerSecond;
	interval_end = atoi(parameters[2].data()) * bytesPerSecond;
	secondStreamNumber = FIRST_FILE;
}

int cv::ChangeBeats::calculateReadPos(const int readPos, int bytesPerSecond) {
	return readPos + bytesPerSecond;
}

void cv::ChangeBeats::countBorders(int& start_index, int& end_index, int stream_start, int stream_end, int buf_size, int elem_size) {
	if (stream_start <= interval_start) start_index = 0;
	else if (stream_start <= interval_end) start_index = stream_start;
	else start_index = interval_end;
	if (stream_end <= interval_start) end_index = 0;
	else if (stream_end <= interval_end) end_index = stream_end;
	else end_index = interval_end;
	start_index = (start_index / elem_size) % buf_size;
	if (end_index != 0) end_index = (end_index / elem_size) % buf_size ? (end_index / elem_size) % buf_size : buf_size;
}

std::string cv::ChangeBeats::returnInfo() {
	return "Converter name: BeatsChanger\nfunction: change 2nd and 4th bits to beats from input file and swap it, beat length is 0.5 seconds\nparamenets:\n\tchangebeats - name of converter\n\tfrom_second - number of second from which changebeats works\n\tto_second - number of second to which changebeats works\nsyntax: changebeats <from_second> <to_second>\n\n";
}

cv::Converter::Converter() {
	interval_start = 0;
	interval_end = 0;
	secondStreamNumber = NO_SECOND_STREAM;
}

void cv::Converter::convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int interval_start, int interval_end) {
	return;
}
void cv::Converter::initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond) {
	return;
}
int cv::Converter::secondInputArg() {
	return secondStreamNumber;
}

void cv::Converter::countBorders(int& start_index, int& end_index, int stream_start, int stream_end, int buf_size, int elem_size) {
	return;
}

int cv::Converter::calculateReadPos(const int readPos, int bytesPerSecond) {
	return readPos;
}

std::string cv::Converter::returnInfo() {
	return "Converter name: --------\nfunction: --------\nparamenets: --------\nsyntax: --------\n";
}

cv::ConverterFactory::~ConverterFactory() {

}
cv::Converter* cv::ConverterFactory::factoryMethod() {
	return new cv::Converter();
}

cv::Converter* cv::MuteFactory::factoryMethod() {
	return new cv::Mute();
}

cv::Converter* cv::MixFactory::factoryMethod() {
	return new cv::Mix();
}

cv::Converter* cv::ChangeBeatsFactory::factoryMethod() {
	return new cv::ChangeBeats();
}

std::vector<cv::ConverterFactory*> cv::ConverterFactory::initialize_array() {
	std::vector<cv::ConverterFactory*> factories;
	factories.push_back(new MixFactory);
	factories.push_back(new MuteFactory);
	factories.push_back(new ChangeBeatsFactory);
	return factories;
}
cv::SoundProcessor::SoundProcessor() {

}

cv::SoundProcessor::~SoundProcessor() {

}

void cv::SoundProcessor::initialize(std::vector<std::vector<std::string>>& config, std::vector<std::vector<std::string>>& filenames) {
	try {
		init_files(filenames);
		init_converters(config);
		input1.resize(wf::DEFAULT_BUF_SIZE);
		input2.resize(wf::DEFAULT_BUF_SIZE);
		output.resize(wf::DEFAULT_BUF_SIZE);
	}
	catch (int errCode) {
		throw errCode;
	}
}

int cv::SoundProcessor::convFind(std::string& convToFind) {
	for (int i = 0; i < CONVERTER_NAMES.size(); i++) {
		if (CONVERTER_NAMES[i] == convToFind) return i;
	}
	return CONVERTER_NAMES.size() + 1;
}

void cv::SoundProcessor::init_files(std::vector<std::vector<std::string>>& filenames) {
	int filesNum = filenames[ac::INPUT_FILES].size() + filenames[ac::OUTPUT_FILES].size();
	try {
		for (int i = 0; i < filesNum - 1; i++) {
			files.push_back(new wavfile::WavFile);
			files[i]->initialize(filenames[ac::INPUT_FILES][i]);
			if (files[i]->isOpen() == NOT_OPENED) {
				delete files[i];
			}
		}
		files.push_back(new wavfile::WavFile);
		files[filesNum - 1]->outInitialize(filenames[ac::OUTPUT_FILES][0]);
	}
	catch (int errCode) {
		throw errCode;
	}
}

void cv::SoundProcessor::init_converters(std::vector<std::vector<std::string>>& config) {
	std::string item;
	cv::ConverterFactory factory;
	std::vector<ConverterFactory*> factories = factory.initialize_array();
	int item_index = 0;
	try {
		for (int i = 0; i < config.size(); i++) {
			item = config[i][0];
			item_index = convFind(item);
			if (item_index != cv::NOT_CONVERTER) {
				converters.push_back(factories[item_index]->factoryMethod());
				converters[converters.size() - 1]->initialize(config[i], wf::BYTES_PER_SECOND);
			}
		}
	}
	catch (int errCode) {
		throw errCode;
	}
}

void cv::SoundProcessor::run() {
	errors::errorCodes errout;
	unsigned long readPos = 0, readPos2 = 0;
	unsigned long writePos = 0;
	unsigned long riddenBytes;
	std::vector<char> header;
	files[files.size() - 1]->changeSize(files[0]->returnHeaderSize() + files[0]->returnDatasize());
	try {
		files[0]->readData(header, readPos, files[0]->returnHeaderSize());
		files[files.size() - 1]->writeData(header, writePos, files[0]->returnHeaderSize());
		writePos = files[0]->returnHeaderSize();
		int secondFileIndex;
		for (;;) {
			riddenBytes = files[0]->readData(input1, readPos + files[0]->returnHeaderSize(), input1.size() * sizeof(unsigned short));
			if (riddenBytes == 0) {
				break;
			}
			for (int j = 0; j < converters.size(); j++) {
				secondFileIndex = converters[j]->secondInputArg();
				if (secondFileIndex != NO_SECOND_STREAM) {
					readPos2 = converters[j]->calculateReadPos(readPos, wf::BYTES_PER_SECOND);
					if (readPos2 >= files[secondFileIndex - 1]->returnDatasize()) {
						continue;
					}
					files[secondFileIndex - 1]->readData(input2, readPos2 + files[secondFileIndex - 1]->returnHeaderSize(), input2.size() * sizeof(unsigned short));
				}
				converters[j]->convert(input1, input2, output, readPos, readPos + input1.size() * sizeof(unsigned short));
				input1.swap(output);
			}
			files[files.size() - 1]->writeData(input1, writePos, riddenBytes);
			writePos += riddenBytes;
			readPos += riddenBytes;
		}
	}
	catch (int errCode) {
		throw errCode;
	}
}

const std::string cv::returnHelpMessage() {
	int NUM_OF_CONVERTERS = 3;
	std::vector<converter::Converter*> convs;
	convs.push_back(new converter::Mute);
	convs.push_back(new converter::Mix);
	convs.push_back(new converter::ChangeBeats);
	std::string helpMessage;
	for (int i = 0; i < NUM_OF_CONVERTERS; i++) {
		helpMessage.append(convs[i]->returnInfo());
	}
	return helpMessage;
}