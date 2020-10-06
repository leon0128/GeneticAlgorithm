#ifndef GA_INPUT_HPP
#define GA_INPUT_HPP

#include <array>

namespace GA
{

class Input
{
public:
    enum
    {
        TURNING_POINT
        , DISPERSION_LOW
        , DISPERSION_HIGH
        , NUM_SPACE_LOW
        , NUM_SPACE_HIGH
        , MAX_HEIGHT_LOW
        , MAX_HEIGHT_HIGH
        , DIFFERENCE_LOW
        , DIFFERENCE_HIGH
        , ARRAY_SIZE
    };

    static const std::array<int, ARRAY_SIZE> MAX_VALUE;


    constexpr Input(const std::array<int, ARRAY_SIZE> &inArr = std::array<int, ARRAY_SIZE>()) noexcept
        : array(inArr){}
    
    int &operator [](std::size_t idx)
        {return array[idx];}
    const int &operator [](std::size_t idx) const
        {return array[idx];}

    std::array<int, ARRAY_SIZE> array;
};

}

#endif