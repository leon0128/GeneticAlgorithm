#include "evaluator.hpp"

namespace GA
{

const int Evaluator::MAX_NUM_USED_BLOCK = 500;

const int Evaluator::USED_BLOCK_MULTIPLE = 10;
const int Evaluator::TETRIS_SCORE_MULTIPLE = 10;
const int Evaluator::THREE_LINES_SCORE_MULTIPLE = 6;
const int Evaluator::TWO_LINES_SCORE_MULTIPLE = 3;
const int Evaluator::ONE_LINE_SCORE_MULTIPLE = 1;

Input Evaluator::INPUT = Input();
Output Evaluator::OUTPUT = Output();

double Evaluator::evaluate(int preMaxHeight
    , double dispersion
    , int numSpace
    , int maxHeight
    , int difference) noexcept
{
    bool isLower = preMaxHeight < INPUT[Input::TURNING_POINT];

    return dispersion * INPUT[isLower ? Input::DISPERSION_LOW : Input::DISPERSION_HIGH]
        + static_cast<double>(numSpace * INPUT[isLower ? Input::NUM_SPACE_LOW : Input::NUM_SPACE_HIGH])
        + static_cast<double>(maxHeight * INPUT[isLower ? Input::MAX_HEIGHT_LOW : Input::MAX_HEIGHT_HIGH])
        + static_cast<double>(difference * INPUT[isLower ? Input::DIFFERENCE_LOW : Input::DIFFERENCE_HIGH]);
}

std::size_t Evaluator::score() noexcept
{
    std::size_t ret = 0;

    ret = OUTPUT[Output::NUM_USED_BLOCK] * USED_BLOCK_MULTIPLE
        + OUTPUT[Output::NUM_ONW_LINE] * ONE_LINE_SCORE_MULTIPLE
        + OUTPUT[Output::NUM_TWO_LINES] * TWO_LINES_SCORE_MULTIPLE
        + OUTPUT[Output::NUM_THREE_LINES] * THREE_LINES_SCORE_MULTIPLE
        + OUTPUT[Output::NUM_TETRIS] * TETRIS_SCORE_MULTIPLE;

    return ret;
}

std::size_t Evaluator::score(const Output &src) noexcept
{
    std::size_t ret = 0;

    ret = src[Output::NUM_USED_BLOCK] * USED_BLOCK_MULTIPLE
        + src[Output::NUM_ONW_LINE] * ONE_LINE_SCORE_MULTIPLE
        + src[Output::NUM_TWO_LINES] * TWO_LINES_SCORE_MULTIPLE
        + src[Output::NUM_THREE_LINES] * THREE_LINES_SCORE_MULTIPLE
        + src[Output::NUM_TETRIS] * TETRIS_SCORE_MULTIPLE;

    return ret;
}

}