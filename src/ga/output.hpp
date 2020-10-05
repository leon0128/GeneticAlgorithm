#ifndef GA_OUTPUT_HPP
#define GA_OUTPUT_HPP

namespace GA
{

class Output
{
public:
    constexpr Output(int inNumUsedBlock = 0
        , int inNumTetris = 0
        , int inNum3Lines = 0
        , int inNum2Lines = 0
        , int inNum1Line = 0) noexcept
        : numUsedBlock(inNumUsedBlock)
        , numTetris(inNumTetris)
        , num3Lines(inNum3Lines)
        , num2Lines(inNum2Lines)
        , num1Line(inNum1Line){}
    
    int numUsedBlock;
    int numTetris;
    int num3Lines;
    int num2Lines;
    int num1Line;
};

}

#endif