#include "evaluator.hpp"

namespace GA
{

const int Evaluator::MAX_NUM_USED_BLOCK = 500;

const int Evaluator::USED_BLOCK_MULTIPLE = 10;
const int Evaluator::TETRIS_SCORE_MULTIPLE = 10;
const int Evaluator::THREE_LINES_SCORE_MULTIPLE = 6;
const int Evaluator::TWO_LINES_SCORE_MULTIPLE = 3;
const int Evaluator::ONE_LINE_SCORE_MULTIPLE = 1;

Input Evaluator::INPUT = Input(0, {1, 1}, {10, 10}, {1, 1}, {1, 1});
Output Evaluator::OUTPUT = Output();

double Evaluator::evaluate(int preMaxHeight
    , double dispersion
    , int numSpace
    , int maxHeight
    , int difference) noexcept
{
    std::size_t idx = preMaxHeight < INPUT.turningPoint ? 0 : 1;

    return dispersion * INPUT.wDispersion[idx]
        + static_cast<double>(numSpace * INPUT.wNumSpace[idx])
        + static_cast<double>(maxHeight * INPUT.wMaxHeight[idx])
        + static_cast<double>(difference * INPUT.wDifference[idx]);
}

std::size_t Evaluator::score() noexcept
{
    std::size_t ret = 0;

    ret = OUTPUT.numUsedBlock * USED_BLOCK_MULTIPLE
        + OUTPUT.num1Line * ONE_LINE_SCORE_MULTIPLE
        + OUTPUT.num2Lines * TWO_LINES_SCORE_MULTIPLE
        + OUTPUT.num3Lines * THREE_LINES_SCORE_MULTIPLE
        + OUTPUT.numTetris * TETRIS_SCORE_MULTIPLE;

    return ret;
}

}