#ifndef GA_EVALUATOR_HPP
#define GA_EVALUATOR_HPP

#include <vector>

#include "input.hpp"
#include "output.hpp"

namespace GA
{

class Evaluator
{
public:
    static int MAX_NUM_USED_BLOCK;
    static int NPC_DEPTH;
    
    static int USED_BLOCK_MULTIPLE;
    static int TETRIS_SCORE_MULTIPLE;
    static int THREE_LINES_SCORE_MULTIPLE;
    static int TWO_LINES_SCORE_MULTIPLE;
    static int ONE_LINE_SCORE_MULTIPLE;

    static Input INPUT;
    static Output OUTPUT;

    Evaluator() = delete;

    static double evaluate(double dispersion
        , int numSpace
        , int maxHeight
        , int difference
        , const std::vector<int> &numDeletedLineVec) noexcept;
    
    static std::size_t score() noexcept;
    static std::size_t score(const Output&) noexcept;
};

}

#endif