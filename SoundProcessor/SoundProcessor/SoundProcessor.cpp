#include <iostream>
#include "Parser.h"
#include "ConfigParser.h"
#include "ArgsContainer.h"
#include "Converters.h"

namespace ps = parser;
namespace cp = configparser;
namespace ac = argscontainer;
namespace cv = converter;

int main(int argc, char ** argv)
{
    ac::ArgsContainer argcont(argc, argv);
    std::vector<std::string> arguments;
    arguments = argcont.returnArguments();
    cp::ConfigParser Configuration;



    return 0;
}

