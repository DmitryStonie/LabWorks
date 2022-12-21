#include <iostream>
#include "WavFile.h"
#include "ConfigParser.h"
#include "ArgsContainer.h"
#include "Converters.h"
#include "Errors.h"

namespace wf = wavfile;
namespace cp = configparser;
namespace ac = argscontainer;
namespace cv = converter;
namespace es = errors;

int main(int argc, char ** argv)
{
    es::ErrorOutput errout;
    try {
        ac::ArgsContainer argcount;
        argcount.readArguments(argc, argv);
        std::vector<std::string> filenames;
        filenames = argcount.returnArguments();
        std::vector<std::string> converterNames = { cv::MIX_STR, cv::MUTE_STR, cv::LOWER_STR };
        cp::ConfigParser configuration;
        configuration.initialize(filenames[filenames.size() - 1], converterNames);
        std::vector<std::vector<std::string>> config = configuration.return_config();
        cv::SoundProcessor soundChanger;
        soundChanger.initialize(config, filenames);
        soundChanger.run(filenames);
    }
    catch (int errCode) {
        errout.writeError(errCode);
    }
    return 0;
}

