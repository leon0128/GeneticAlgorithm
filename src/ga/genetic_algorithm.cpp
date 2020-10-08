#include <map>
#include <iostream>

#include "json.hpp"
#include "evaluator.hpp"
#include "random.hpp"
#include "genetic_algorithm.hpp"

namespace GA
{

std::string OUTPUT_FILENAME;

GeneticAlgorithm GeneticAlgorithm::GA;

const double GeneticAlgorithm::CROSSOVER_RATE = 0.6;
const double GeneticAlgorithm::MUTATION_RATE = 0.1;
const std::size_t GeneticAlgorithm::NUM_ELEMENT = 30;
const std::size_t GeneticAlgorithm::NUM_GENERATION = 10;
const bool GeneticAlgorithm::IS_SELECTED_ELITE = true;
const std::vector<double> GeneticAlgorithm::RANKING_PROBABILITY
    = {0.15 , 0.15, 0.15, 0.15, 0.1, 0.1, 0.1, 0.1};
const std::size_t GeneticAlgorithm::TOURNAMENT_SIZE = 3;
const std::size_t GeneticAlgorithm::NUM_ELITE = 2;
const std::size_t GeneticAlgorithm::NUM_POINT = 3;
const GeneticAlgorithm::CrossingTag GeneticAlgorithm::CROSSING_TAG = GeneticAlgorithm::CrossingTag::UNIFORM_CROSSING;
const GeneticAlgorithm::SelectionTag GeneticAlgorithm::SELECTION_TAG = GeneticAlgorithm::SelectionTag::TOURNAMENT_SELECTION;

GeneticAlgorithm::GeneticAlgorithm()
    : mIOVec()
    , mIdx(0)
    , mGen(1)
    , mJson(new Json())
{
    initialize();
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    delete mJson;
}

void GeneticAlgorithm::initialize()
{
    for(std::size_t i = 0; i < NUM_ELEMENT; i++)
        mIOVec.emplace_back(generateRandom(), Output());
}

void GeneticAlgorithm::evolve()
{
    std::vector<std::pair<Input, Output>> nextIO;

    if(IS_SELECTED_ELITE)
    {
        std::multimap<std::size_t, Input> map;
        for(std::size_t i = 0; i < mIOVec.size(); i++)
            map.emplace(Evaluator::score(mIOVec[i].second), mIOVec[i].first);
        
        std::size_t idx = 0;
        for(auto &&iter = map.rbegin();
            iter != map.rend() && idx < NUM_ELITE;
            iter++, idx++)
            nextIO.emplace_back(iter->second, Output());
    }

    for(std::size_t i = nextIO.size(); i < NUM_ELEMENT; i++)
    {
        double rand = Random::random();
        double prob = 0.0;
        if((prob += CROSSOVER_RATE) > rand)
            nextIO.emplace_back(generateCrossing(select(), select()), Output());
        else if((prob += MUTATION_RATE) > rand)
            nextIO.emplace_back(generateMutation(select()), Output());
        else
            nextIO.emplace_back(select(), Output());
    }

    mIOVec.swap(nextIO);
}

void GeneticAlgorithm::setInputToEvaluator()
{
    if(mIdx == NUM_ELEMENT)
    {
        mIdx = 0;
        mGen++;
        evolve();
    }

    Evaluator::INPUT = mIOVec[mIdx].first;
    // Evaluator::INPUT = Input({15, 70, 15, 15, 99});
}

void GeneticAlgorithm::getOutputFromEvaluator()
{
    mIOVec[mIdx++].second = Evaluator::OUTPUT;

    mJson->add(mGen, mIdx - 1, Evaluator::INPUT, Evaluator::OUTPUT);
}

Input GeneticAlgorithm::select() const
{
    Input ret;

    switch(SELECTION_TAG)
    {
        case(SelectionTag::ROULETTE_SELECTION):
        {
            std::size_t sumScore = 0;
            std::vector<std::size_t> scores;
            for(auto &&p : mIOVec)
            {
                scores.push_back(Evaluator::score(p.second));
                sumScore += scores.back();
            }

            double prob = 0.0;
            double rand = Random::random();
            for(std::size_t i = 0; i < mIOVec.size(); i++)
            {
                if((prob += static_cast<double>(scores[i]) / static_cast<double>(sumScore)) > rand)
                {
                    ret = mIOVec[i].first;
                    break;
                }

                if(i + 1 == mIOVec.size())
                    ret = mIOVec.back().first;
            }

            break;
        }
        case(SelectionTag::RANKING_SELECTION):
        {
            std::multimap<std::size_t, const Input&> map;
            for(auto &&p : mIOVec)
                map.emplace(Evaluator::score(p.second), p.first);
            
            double sumProb = 0.0;
            double prob = Random::random();
            std::size_t idx = 0;
            for(auto &&iter = map.rbegin();
                iter != map.rend() && idx < RANKING_PROBABILITY.size();
                iter++, idx++)
            {
                if((sumProb += RANKING_PROBABILITY[idx]) > prob)
                {
                    ret = iter->second;
                    break;
                }
            }

            break;
        }
        case(SelectionTag::TOURNAMENT_SELECTION):
        {
            std::vector<std::size_t> selectionVec;
            for(std::size_t i = 0; i < mIOVec.size(); i++)
                selectionVec.push_back(i);
            
            std::size_t score = 0;
            for(std::size_t i = 0; i < mIOVec.size() && i < TOURNAMENT_SIZE; i++)
            {
                std::size_t idx = static_cast<std::size_t>(Random::random() * static_cast<double>(selectionVec.size()));
                std::size_t s = Evaluator::score(mIOVec[selectionVec[idx]].second);
                if(s > score)
                {
                    score = s;
                    ret = mIOVec[selectionVec[idx]].first;
                }
                selectionVec.erase(selectionVec.begin() + idx);
            }

            break;
        }
    }

    std::cout << "select:\n"
        "    ";
    ret.print();

    return ret;
}

Input GeneticAlgorithm::generateRandom() const
{
    Input ret = Input({static_cast<int>(Random::random() * Input::MAX_VALUE[Input::DISPERSION_LOW])
        , static_cast<int>(Random::random() * Input::MAX_VALUE[Input::NUM_SPACE_LOW])
        , static_cast<int>(Random::random() * Input::MAX_VALUE[Input::MAX_HEIGHT_LOW])
        , static_cast<int>(Random::random() * Input::MAX_VALUE[Input::DIFFERENCE_LOW])
        , static_cast<int>(Random::random() * Input::MAX_VALUE[Input::NUM_DELETED_LINE])});

    std::cout << "gen-random:\n"
        "    ";
    ret.print();

    return ret;
}

Input GeneticAlgorithm::generateCrossing(const Input &lhs, const Input &rhs) const
{
    Input ret = lhs;

    switch(CROSSING_TAG)
    {
        case(CrossingTag::ONE_POINT_CROSSING):
        {
            std::size_t bp = static_cast<std::size_t>(Random::random() * static_cast<double>(Input::ARRAY_SIZE));
            for(std::size_t i = 0; i < Input::ARRAY_SIZE; i++)
            {
                if(bp == 0)
                    ret[i] = rhs[i];
                else
                    bp--;
            }

            break;
        }
        case(CrossingTag::TWO_POINT_CROSSING):
        {
            std::array<std::size_t, 2> bp;
            bp[0] = static_cast<std::size_t>(Random::random() * static_cast<double>(Input::ARRAY_SIZE));
            bp[1] = static_cast<std::size_t>(Random::random() * static_cast<double>(Input::ARRAY_SIZE));

            for(std::size_t i = 0; i < Input::ARRAY_SIZE; i++)
            {
                if(bp[0] == 0)
                {
                    if(bp[1] == 0)
                        break;
                    else
                    {
                        bp[1]--;
                        ret[i] = rhs[i];
                    }
                }
                else
                {
                    if(bp[1] == 0)
                    {
                        bp[0]--;
                        ret[i] = rhs[i];
                    }
                    else
                    {
                        bp[0]--;
                        bp[1]--;
                    }
                }

            }

            break;
        }
        case(CrossingTag::MULTI_POINT_CROSSING):
        {
            std::vector<std::size_t> bp;
            for(std::size_t i = 0; i < NUM_POINT; i++)
                bp.push_back(static_cast<std::size_t>(Random::random() * static_cast<double>(Input::ARRAY_SIZE)));

            for(std::size_t i = 0; i < Input::ARRAY_SIZE; i++)
            {
                int numZero = 0;
                for(auto &&e : bp)
                {
                    if(e == 0)
                        numZero++;
                    else
                        e--;
                }

                if(numZero % 2 == 1)
                    ret[i] = rhs[i];
            }

            break;
        }
        case(CrossingTag::UNIFORM_CROSSING):
        {
            for(std::size_t i = 0; i < Input::ARRAY_SIZE; i++)
            {
                if(Random::random() < 0.5)
                    ret[i] = rhs[i];
            }

            break;
        }
    }

    std::cout << "gen-crossing:\n"
        "    ";
    lhs.print();
    std::cout << "    ";
    rhs.print();
    std::cout << "    ";
    ret.print();

    return ret;
}

Input GeneticAlgorithm::generateMutation(const Input &src) const
{
    Input ret = generateRandom();
    
    std::cout << "gen-mutation:\n"
        "    ";
    ret.print();

    return ret;
}

}