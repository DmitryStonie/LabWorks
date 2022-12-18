#include "Converters.h"
#include <limits.h>

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

cv::Converter* cv::MuteFactory::factoryMethod() {
	return new cv::Mute();
}

cv::Converter* cv::MixFactory::factoryMethod() {
	return new cv::Mix();
}

cv::Converter* cv::LowerFactory::factoryMethod() {
	return new cv::Lower();
}