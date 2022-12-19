#include "Converters.h"
#include "WavFile.h"
#include <limits.h>
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

void cv::Mix::initialize(std::vector<std::string> parameters) {
	interval_start = atoi(parameters[2].data());
	interval_end = END_OF_FILE;
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

void cv::Mute::initialize(std::vector<std::string> parameters) {
	interval_start = atoi(parameters[1].data());
	interval_end = atoi(parameters[2].data());
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

void cv::Lower::initialize(std::vector<std::string> parameters) {
	interval_start = atoi(parameters[1].data());
	interval_end = atoi(parameters[2].data());
}

int cv::Lower::secondInputArg() {
	return NO_SECOND_STREAM;
}

void cv::Converter::convert(std::vector<unsigned short>& input1, std::vector<unsigned short>& input2, std::vector<unsigned short>& output, int interval_start, int interval_end) {
	return;
}
void cv::Converter::initialize(std::vector<std::string> parameters) {
	return;
}
int cv::Converter::secondInputArg() {
	return NO_SECOND_STREAM;
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

void cv::SoundProcessor::initialize(std::vector<std::vector<std::string>> config, std::vector<std::string> filenames) {
	init_converters(config);
	init_files(filenames);
}

int cv::SoundProcessor::convFind(std::string convToFind, std::vector<std::string> &converterNames) {
	for (int i = 0; i < converterNames.size(); i++) {
		if (converterNames[i] == convToFind) return i;
	}
	return converterNames.size() + 1;
}

void cv::SoundProcessor::init_files(std::vector<std::string> filenames) {
	for (int i = 0; i < filenames.size() - 1; i++) {
		files[i] = new wavfile::WavFile;
		files[i]->initialize(filenames[i]);
		if (files[i]->isOpen() == NOT_OPENED) {
			delete files[i];
			files[i] == NULL;
		}
	}
}

void cv::SoundProcessor::init_converters(std::vector<std::vector<std::string>> config) {
	std::vector<std::string> converterNames = { MIX_STR, MUTE_STR, LOWER_STR };
	std::string item;
	cv::ConverterFactory factory;
	std::vector<ConverterFactory*> factories = factory.initialize_array();
	int item_index = 0;
	for (int i = 0; i < config.size(); i++) {
		item = config[i][0];
		item_index = convFind(item, converterNames);
		if (item_index != cv::NOT_CONVERTER) {
			converters.push_back(factories[item_index]->factoryMethod());
			converters[converters.size() - 1]->initialize(config[i]);
		}
	}
}

void cv::SoundProcessor::run(std::vector<std::string> fileNames) {
	wf::WavFile outputFile;
	outputFile.initialize(fileNames[files.size() - 2]);
	outputFile.writeHeader();

}
