#ifndef GA_INPUT_HPP
#define GA_INPUT_HPP

#include <array>
#include <utility>
#include <string>

namespace GA
{

class Input
{
public:
    static Input INIT;

    enum
    {
        DISPERSION_LOW
        , NUM_SPACE_LOW
        , MAX_HEIGHT_LOW
        , DIFFERENCE_LOW
        , NUM_DELETED_LINE
        , ARRAY_SIZE
    };

    Input(const std::array<double, ARRAY_SIZE> &inArr = std::array<double, ARRAY_SIZE>())
        : array(inArr){}
    
    std::string str() const;

    double &operator [](std::size_t idx)
        {return array[idx];}
    const double &operator [](std::size_t idx) const
        {return array[idx];}

    std::array<double, ARRAY_SIZE> array;
};

}

#endif