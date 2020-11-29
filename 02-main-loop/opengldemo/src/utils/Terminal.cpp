#include "utils/Terminal.h"

#include <iomanip>
#include <iostream>

bool Terminal::is_inited = false;

Terminal::Terminal(){}

void Terminal::Report(std::string_view msg)
{
    std::cerr << msg << std::endl;
}

void Terminal::PrintBorderedText(const char filler, std::string_view msg)
{

    auto Line = [filler]() {
        std::cout.setf(std::ios::left, std::ios::adjustfield);
        std::cout.fill(filler);
        std::cout << std::setw(WIDTH) << filler << std::endl;
    };
    auto Empty = [filler]() {
        std::cout.setf(std::ios::right, std::ios::adjustfield);
        std::cout.fill(' ');
        std::cout << filler << std::setw(WIDTH - 1) << filler << std::endl;
    };

    auto Text = [filler](std::string_view msg) {
        if (msg.length() > WIDTH - 4)
            std::cout << filler << ' ' << msg.substr(0, 46) << ' ' << filler
                      << std::endl;
        else
        {
            const int shift = msg.length() & 1 ? 1 : 0;
            std::cout.fill(' ');
            auto empty = WIDTH / 2 - shift - msg.length() / 2;
            std::cout.setf(std::ios::left, std::ios::adjustfield);
            std::cout << std::setw(empty) << filler;
            std::cout << msg;
            std::cout.setf(std::ios::right, std::ios::adjustfield);
            std::cout << std::setw(empty + shift) << filler;
            std::cout << std::endl;
        }
    };

    Line();
    Empty();
    Text(msg);
    Empty();
    Line();
}
