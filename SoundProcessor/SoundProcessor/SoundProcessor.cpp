#include <iostream>
#include "WavFile.h"
#include "ConfigParser.h"
#include "ArgsContainer.h"
#include "Converters.h"

namespace wf = wavfile;
namespace cp = configparser;
namespace ac = argscontainer;
namespace cv = converter;

int main(int argc, char ** argv)
{
    ac::ArgsContainer argcont(argc, argv);
    std::vector<std::string> filenames;
    filenames = argcont.returnArguments();
    std::vector<std::string> converterNames = { cv::MIX_STR, cv::MUTE_STR, cv::LOWER_STR };
    cp::ConfigParser configuration(filenames[filenames.size() - 1], converterNames);
    std::vector<std::vector<std::string>> config = configuration.return_config();
    cv::SoundProcessor soundChanger;
    soundChanger.initialize(config, filenames);
    soundChanger.run(filenames);
    return 0;
}

