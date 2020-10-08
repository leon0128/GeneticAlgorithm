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
        DISPERSION_LOW
        , NUM_SPACE_LOW
        , MAX_HEIGHT_LOW
        , DIFFERENCE_LOW
        , NUM_DELETED_LINE
        , ARRAY_SIZE
    };

    static const std::array<int, ARRAY_SIZE> MAX_VALUE;


    constexpr Input(const std::array<int, ARRAY_SIZE> &inArr = std::array<int, ARRAY_SIZE>()) noexcept
        : array(inArr){}
    
    void print() const;

    int &operator [](std::size_t idx)
        {return array[idx];}
    const int &operator [](std::size_t idx) const
        {return array[idx];}

    std::array<int, ARRAY_SIZE> array;
};

}

#endif