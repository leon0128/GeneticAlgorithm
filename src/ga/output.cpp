#include <iostream>

#include "output.hpp"

namespace GA
{

void Output::print() const
{
    std::cout << "output: {";

    for(auto &&i : array)
        std::cout << i << ", ";
    
    std::cout << "}\n"
        << std::flush;
}

}