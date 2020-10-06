#ifndef GA_GENETIC_ALGORITHM_HPP
#define GA_GENETIC_ALGORITHM_HPP

#include <utility>
#include <vector>
#include <string>

#include "input.hpp"
#include "output.hpp"

namespace GA
{

extern std::string OUTPUT_FILENAME;

class Json;

class GeneticAlgorithm
{
public:
    enum class CrossingTag;
    enum class SelectionTag;

    static GeneticAlgorithm GA;

    static const double CROSSOVER_RATE;
    static const double MUTATION_RATE;
    static const std::size_t NUM_ELEMENT;
    static const std::size_t NUM_GENERATION;
    static const bool IS_SELECTED_ELITE;
    static const std::vector<double> RANKING_PROBABILITY;
    static const std::size_t TOURNAMENT_SIZE;
    static const std::size_t NUM_ELITE;
    static const std::size_t NUM_POINT; // for multi-point crossing
    static const CrossingTag CROSSING_TAG;
    static const SelectionTag SELECTION_TAG;

    GeneticAlgorithm();
    ~GeneticAlgorithm();

    void initialize();
    void evolve();

    void setInputToEvaluator();
    void getOutputFromEvaluator();

    bool end() const noexcept
        {return mIdx == NUM_ELEMENT && mGen == NUM_GENERATION;}
    

private:
    Input select() const;

    Input generateRandom() const;
    Input generateCrossing(const Input&, const Input&) const;
    Input generateMutation(const Input&) const;

    std::vector<std::pair<Input, Output>> mIOVec;
    std::size_t mIdx;
    std::size_t mGen;

    Json *mJson;
};

enum class GeneticAlgorithm::CrossingTag
{
    ONE_POINT_CROSSING
    , TWO_POINT_CROSSING
    , MULTI_POINT_CROSSING
    , UNIFORM_CROSSING
};

enum class GeneticAlgorithm::SelectionTag
{
    ROULETTE_SELECTION
    , RANKING_SELECTION
    , TOURNAMENT_SELECTION
};

}

#endif