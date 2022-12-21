#include "Converters.h"
#include "WavFile.h"
#include "Errors.h"
#include <limits>
#include <algorithm>

namespace cv = converter;
namespace wf = wavfile;

void cv::Mix::convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int stream_start, int stream_end) {
	int start_index = 0, end_index = 0, iter_count = 0;
	if (stream_start <= interval_start) start_index = interval_start;
	else start_index = stream_start;
	if (stream_end <= interval_start) return;
	else end_index = stream_end;
	iter_count = end_index - start_index;
	for (int i = start_index - stream_start, k = 0; k < iter_count; k++) {
		output[i + k] = (input1[i + k] + input2[i + k]) / 2;
	}
}

void cv::Mix::initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond) {
	if (parameters.size() < 3) return;
	interval_start = atoi(parameters[2].data()) * bytesPerSecond;
	interval_end = MAX_DATA_SIZE;
	secondStreamNumber = atoi((parameters[1].substr(1)).data());
}

int cv::Mix::secondInputArg() {
	return secondStreamNumber;
}

void cv::Mute::convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int stream_start, int stream_end) {
	int start_index = 0, end_index = 0, iter_count = 0;
	if (stream_start <= interval_start) start_index = interval_start;
	else if (stream_start < interval_end) start_index = stream_start;
	else return;
	if (stream_end <= interval_start) return;
	else if (stream_end < interval_end) end_index = stream_end;
	else end_index = interval_end;
	iter_count = end_index - start_index;
	std::fill(output.begin() + start_index, output.end() - (input1.size() - end_index), ZERO);
}

void cv::Mute::initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond) {
	if (parameters.size() < 3) return;
	interval_start = atoi(parameters[1].data()) * bytesPerSecond;
	interval_end = atoi(parameters[2].data()) * bytesPerSecond;
}

int cv::Mute::secondInputArg() {
	return NO_SECOND_STREAM;
}

void cv::Lower::convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int stream_start, int stream_end) {
	int start_index = 0, end_index = 0, iter_count = 0;
	if (stream_start <= interval_start) start_index = interval_start;
	else if (stream_start < interval_end) start_index = stream_start;
	else return;
	if (stream_end <= interval_start) return;
	else if (stream_end < interval_end) end_index = stream_end;
	else end_index = interval_end;
	iter_count = end_index - start_index;
	const unsigned int HIGHER = 10000;
	for (int i = start_index - stream_start, k = 0; k < iter_count; k++) {
		output[i] = (input1[i] + HIGHER) % USHRT_MAX;
	}
}

void cv::Lower::initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond) {
	if (parameters.size() < 3) return;	//errors, need to destroy converter or not use it
	interval_start = atoi(parameters[1].data()) * bytesPerSecond;
	interval_end = atoi(parameters[2].data()) * bytesPerSecond;
}

int cv::Lower::secondInputArg() {
	return NO_SECOND_STREAM;
}

void cv::Converter::convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int interval_start, int interval_end) {
	return;
}
void cv::Converter::initialize(std::vector<std::string>& parameters, unsigned long bytesPerSecond) {
	return;
}
int cv::Converter::secondInputArg() {
	return NO_SECOND_STREAM;
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

cv::Converter* cv::LowerFactory::factoryMethod() {
	return new cv::Lower();
}

std::vector<cv::ConverterFactory*> cv::ConverterFactory::initialize_array() {
	std::vector<cv::ConverterFactory*> factories;
	factories.push_back(new MixFactory);
	factories.push_back(new MuteFactory);
	factories.push_back(new LowerFactory);
	return factories;
}
cv::SoundProcessor::SoundProcessor() {

}

cv::SoundProcessor::~SoundProcessor() {

}

void cv::SoundProcessor::initialize(std::vector<std::vector<std::string>>& config, std::vector<std::string>& filenames) {
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

int cv::SoundProcessor::convFind(std::string& convToFind, std::vector<std::string> &converterNames) {
	for (int i = 0; i < converterNames.size(); i++) {
		if (converterNames[i] == convToFind) return i;
	}
	return converterNames.size() + 1;
}

void cv::SoundProcessor::init_files(std::vector<std::string>& filenames) {
	try {
		for (int i = 0; i < filenames.size() - 2; i++) {
			files.push_back(new wavfile::WavFile);
			files[i]->initialize(filenames[i]);
			if (files[i]->isOpen() == NOT_OPENED) {
				delete files[i];
				files[i] == NULL;
			}
		}
		files.push_back(new wavfile::WavFile);
		files[filenames.size() - 2]->outInitialize(filenames[filenames.size() - 2]);
		files[files.size() - 1]->setDefaultHeader();
	}
	catch (int errCode) {
		throw errCode;
	}
}

void cv::SoundProcessor::init_converters(std::vector<std::vector<std::string>>& config) {
	std::vector<std::string> converterNames = { MIX_STR, MUTE_STR, LOWER_STR };
	std::string item;
	cv::ConverterFactory factory;
	std::vector<ConverterFactory*> factories = factory.initialize_array();
	int item_index = 0;
	try {
		for (int i = 0; i < config.size(); i++) {
			item = config[i][0];
			item_index = convFind(item, converterNames);
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
int cv::SoundProcessor::countMaxSize() {
	unsigned long tmpSize = UINT32_MAX, tmp = 0;
	for (int i = 0; i < files.size() - 1; i++) {
		tmp = files[i]->returnDatasize();
		if (tmp < tmpSize) tmpSize = tmp;
	}
	return tmpSize;
}

void cv::SoundProcessor::run(std::vector<std::string>& fileNames) {
	errors::errorCodes errout;
	unsigned long readPos = 0;
	unsigned long writePos = 0;
	unsigned long riddenBytes;
	int outputFileSize = (*this).countMaxSize();
	files[files.size() - 1]->changeSize(wf::DEFAULT_HEADER_SIZE + outputFileSize);
	try{
		files[files.size() - 1]->writeHeader();
		writePos += files[files.size() - 1]->returnHeadersize();
		int secondFileIndex;
		for (;;) {
			riddenBytes = files[0]->readData(input1, readPos);
			if (riddenBytes == 0) break;
			for (int j = 0; j < converters.size(); j++) {
				secondFileIndex = converters[j]->secondInputArg();
				if (secondFileIndex != NO_SECOND_STREAM) {
					files[secondFileIndex]->readData(input2, readPos);
				}
				converters[j]->convert(input1, input2, output, readPos, readPos + input1.size());
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
