#include <iostream>
#include "Parser.h"
#include "ConfigParser.h"
#include "ArgsContainer.h"
#include "Converters.h"

int main(int argc, char ** argv)
{
    std::string filename = "severe_tire_damage.wav";
    parser::Header header(filename);
    if (header.return_correctness() == parser::INCORRECT) {
        std::cerr << "Cannot use file " << filename << "\n";
    }
    

}

