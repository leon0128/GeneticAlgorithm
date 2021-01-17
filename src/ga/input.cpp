#include <iostream>
#include <limits>

#include "input.hpp"

namespace GA
{

Input Input::INIT = Input({0.0, 0.0, 0.0, 0.0, 0.0});

std::string Input::str() const
{
    std::string ret = "input: [";
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