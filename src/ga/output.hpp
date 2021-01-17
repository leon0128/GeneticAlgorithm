#ifndef GA_OUTPUT_HPP
#define GA_OUTPUT_HPP

#include <array>
#include <string>

namespace GA
{

class Output
{
public:
    static Output INIT;

    enum
    {
        NUM_USED_BLOCK
        , NUM_TETRIS
        , NUM_THREE_LINES
        , NUM_TWO_LINES
        , NUM_ONW_LINE
        , ARRAY_SIZE
    };

    Output(const std::array<int, ARRAY_SIZE> &inArr = std::array<int, ARRAY_SIZE>())
        : array(inArr){}
    
    std::string str() const;

    int &operator [](std::size_t idx)
        {return array[idx];}
    const int &operator [](std::size_t idx) const
        {return array[idx];}
    
    std::array<int, ARRAY_SIZE> array;
};

}

#endif