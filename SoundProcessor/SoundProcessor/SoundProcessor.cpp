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
    std::vector<std::string> arguments;
    arguments = argcont.returnArguments();
    for(int i = 0; i < arguments.size(); i++) std::cout << arguments[i] << '\t';
    wf::WavFile outputFile();
    //cp::ConfigParser Configuration;



    return 0;
}

