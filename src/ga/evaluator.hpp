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
    inline static int MAX_NUM_USED_BLOCK = 500;
    inline static int NPC_DEPTH = 2;
    
    inline static int USED_BLOCK_MULTIPLE = 10;
    inline static int TETRIS_SCORE_MULTIPLE = 64;
    inline static int THREE_LINES_SCORE_MULTIPLE = 8;
    inline static int TWO_LINES_SCORE_MULTIPLE = 4;
    inline static int ONE_LINE_SCORE_MULTIPLE = 1;

    inline static Input INPUT = Input::INIT;
    inline static Output OUTPUT = Output::INIT;

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