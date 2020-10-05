#ifndef GA_INPUT_HPP
#define GA_INPUT_HPP

#include <array>

namespace GA
{

class Input
{
public:
    constexpr Input(int inTurningPoint = 0
        , const std::array<int, 2> &inWDispersion = {1, 1}
        , const std::array<int, 2> &inWNumSpace = {1, 1}
        , const std::array<int, 2> &inWMaxHeight = {1, 1}
        , const std::array<int, 2> &inWDifference = {1, 1}) noexcept
        : turningPoint(inTurningPoint)
        , wDispersion(inWDispersion)
        , wNumSpace(inWNumSpace)
        , wMaxHeight(inWMaxHeight)
        , wDifference(inWDifference){}

    int turningPoint;

    std::array<int, 2> wDispersion;
    std::array<int, 2> wNumSpace;
    std::array<int, 2> wMaxHeight;
    std::array<int, 2> wDifference;
};

}

#endif