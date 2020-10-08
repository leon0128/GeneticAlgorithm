#include <iostream>

#include "input.hpp"

namespace GA
{

const std::array<int, Input::ARRAY_SIZE> Input::MAX_VALUE
    = {100
        , 100
        , 100
        , 100
        , 100};

void Input::print() const
{
    std::cout << "input: {";

    for(auto &&i : array)
        std::cout << i << ", ";
    
    std::cout << "}\n"
        << std::flush;
}

}