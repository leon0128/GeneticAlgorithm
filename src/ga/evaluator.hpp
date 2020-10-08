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
    static const int MAX_NUM_USED_BLOCK;
    
    static const int USED_BLOCK_MULTIPLE;
    static const int TETRIS_SCORE_MULTIPLE;
    static const int THREE_LINES_SCORE_MULTIPLE;
    static const int TWO_LINES_SCORE_MULTIPLE;
    static const int ONE_LINE_SCORE_MULTIPLE;

    static Input INPUT;
    static Output OUTPUT;

    Evaluator() = delete;

    static double evaluate(int preMaxHeight
        , double dispersion
        , int numSpace
        , int maxHeight
        , int difference
        , int numDeletedLine) noexcept;
    
    static std::size_t score() noexcept;
    static std::size_t score(const Output&) noexcept;
};

}

#endif