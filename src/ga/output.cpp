#include <iostream>

#include "output.hpp"

namespace GA
{

Output Output::INIT = Output({0, 0, 0, 0, 0});

std::string Output::str() const
{
    std::string ret = "output: [";
    if(!array.empty())
        ret += std::to_string(array.front());
    
    for(std::size_t i = 1; i < array.size(); i++)
    {
        ret += ", ";
        ret += std::to_string(array[i]);
    }

    ret.push_back(']');

    return ret;
}

}