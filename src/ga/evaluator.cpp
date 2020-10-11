#include "evaluator.hpp"

namespace GA
{

int Evaluator::MAX_NUM_USED_BLOCK = 500;
int Evaluator::NPC_DEPTH = 2;

int Evaluator::USED_BLOCK_MULTIPLE = 10;
int Evaluator::TETRIS_SCORE_MULTIPLE = 64;
int Evaluator::THREE_LINES_SCORE_MULTIPLE = 8;
int Evaluator::TWO_LINES_SCORE_MULTIPLE = 4;
int Evaluator::ONE_LINE_SCORE_MULTIPLE = 1;

Input Evaluator::INPUT = Input();
Output Evaluator::OUTPUT = Output();

double Evaluator::evaluate(double dispersion
    , int numSpace
    , int maxHeight
    , int difference
    , const std::vector<int> &numDeletedLineVec) noexcept
{
    double ret = 0.0;

    for(auto &&i : numDeletedLineVec)
    {
        double tmp = 0.0;
        switch(i)
        {
            case(1):
                tmp += static_cast<double>(ONE_LINE_SCORE_MULTIPLE);
                break;
            case(2):
                tmp += static_cast<double>(TWO_LINES_SCORE_MULTIPLE);
                break;
            case(3):
                tmp += static_cast<double>(THREE_LINES_SCORE_MULTIPLE);
                break;
            case(4):
                tmp += static_cast<double>(TETRIS_SCORE_MULTIPLE);
                break;
            
            default:;
                tmp += static_cast<double>(TETRIS_SCORE_MULTIPLE + THREE_LINES_SCORE_MULTIPLE + TWO_LINES_SCORE_MULTIPLE + ONE_LINE_SCORE_MULTIPLE) / 4.0;
        }

        ret += 1.0 / tmp * static_cast<double>(INPUT[Input::NUM_DELETED_LINE]);
    }

    ret += dispersion * INPUT[Input::DISPERSION_LOW]
        + static_cast<double>(numSpace * INPUT[Input::NUM_SPACE_LOW])
        + static_cast<double>(maxHeight * INPUT[Input::MAX_HEIGHT_LOW])
        + static_cast<double>(difference * INPUT[Input::DIFFERENCE_LOW]);

    return ret;
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