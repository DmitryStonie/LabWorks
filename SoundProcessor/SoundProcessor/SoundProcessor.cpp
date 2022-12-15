#include <iostream>
#include "Parser.h"

int main()

{
    std::string filename = "severe_tire_damage.wav";
    parser::Header header(filename);
    if (header.return_correctness() == parser::INCORRECT) {
        std::cerr << "Cannot use file " << filename << "\n";
    }
}

