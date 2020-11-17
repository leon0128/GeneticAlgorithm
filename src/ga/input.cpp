#include <iostream>
#include <limits>

#include "input.hpp"

namespace GA
{

Input Input::INIT;

const std::array<double, Input::ARRAY_SIZE> Input::MAX_VALUE
    = {1.0
        , 1.0
        , 1.0
        , 1.0
        , 1.0};

void Input::print() const
{
    std::cout << "input: {";

    for(auto &&i : array)
        std::cout << i << ", ";
    
    std::cout << "}\n"
        << std::flush;
}

}