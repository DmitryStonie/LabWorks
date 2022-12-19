#include "Converters.h"
#include <limits.h>
#include <algorithm>

namespace cv = converter;

void cv::Mix::convert(std::vector<unsigned short> &input, std::vector<unsigned short> &output) {
	for (int i = 0; i < input.size(); i++) {
		output[i] = (input[i] + output[i]) / 2;
	}
}

void cv::Mute::convert(std::vector<unsigned short> &input, std::vector<unsigned short> &output) {
	unsigned short ZERO = 0;
	std::fill(output.begin(), output.end(), ZERO);
}

void cv::Lower::convert(std::vector<unsigned short> &input, std::vector<unsigned short> &output) {
	output = input;
	const unsigned int HIGHER = 10000;
	for (int i = 0; i < input.size(); i+=2) {
		output[i] = (output[i] + HIGHER) % USHRT_MAX;
	}
}
void cv::Converter::convert(std::vector<unsigned short>& input, std::vector<unsigned short>& output) {
	return;
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

cv::SoundProcessor::SoundProcessor(std::vector<std::vector<std::string>> configFile) {
	std::vector<std::string> converterNames = { MIX_STR, MUTE_STR, LOWER_STR };
	cv::ConverterFactory factory;
	std::vector<ConverterFactory*> factories = factory.initialize_array();
	int item_index = 0;
	std::string item;
	
	for (int i = 0; i < configFile.size(); i++) {
		item = configFile[i][0];
		//item_index = convFind(item);
		if (item_index != cv::NOT_CONVERTER) {
			converters.push_back(factories[item_index]->factoryMethod());
			convertings.push_back(read_arguments(configFile[i], item_index));
		}
	}

}

int cv::SoundProcessor::convFind(std::string convToFind, std::vector<std::string> &converterNames) {
	for (int i = 0; i < converterNames.size(); i++) {
		if (converterNames[i] == convToFind) return i;
	}
	return converterNames.size() + 1;
}

std::vector<int> cv::SoundProcessor::read_arguments(std::vector<std::string> source, int converterCode) {
	std::vector<int> arguments;
	arguments.push_back(converterCode);
	std::string item;
	//int 
	for (int i = 1; i < source.size(); i++) {
		item = source[i];
		if (item[0] == '$') {

		}
	}
	return arguments;
}

void cv::SoundProcessor::run() {

}
