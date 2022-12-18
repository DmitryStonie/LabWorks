#pragma once
#include <vector>


namespace converter {
	class Converter {
	public:
		virtual void convert(std::vector<unsigned short>& input, std::vector<unsigned short>& output) = 0;
		virtual ~Converter() {}
	};

	class Mix : public Converter {
		void convert(std::vector<unsigned short>& input, std::vector<unsigned short>& output);
	};

	class Mute : public Converter {
		void convert(std::vector<unsigned short>& input, std::vector<unsigned short>& output);
	};

	class Lower : public Converter {
		void convert(std::vector<unsigned short>& input, std::vector<unsigned short>& output);
	};

	class ConverterFactory {
	public:
		virtual Converter* factoryMethod() = 0;
	};

	class MixFactory : public ConverterFactory{
		Converter* factoryMethod();
	};

	class MuteFactory : public ConverterFactory{
		Converter* factoryMethod();

	};

	class LowerFactory : public ConverterFactory {
		Converter* factoryMethod();
	};
}