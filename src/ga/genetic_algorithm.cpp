#include <map>
#include <limits>
#include <numeric>
#include <algorithm>
#include <ios>
#include <iomanip>
#include <iostream>

#include "json.hpp"
#include "evaluator.hpp"
#include "random.hpp"
#include "genetic_algorithm.hpp"

namespace GA
{

std::string OUTPUT_FILENAME;

GeneticAlgorithm GeneticAlgorithm::GA;

double GeneticAlgorithm::CROSSOVER_RATE = 0.6;
double GeneticAlgorithm::MUTATION_RATE = 0.1;
std::size_t GeneticAlgorithm::NUM_ELEMENT = 30;
std::size_t GeneticAlgorithm::NUM_GENERATION = 10;
bool GeneticAlgorithm::IS_SELECTED_ELITE = true;
std::vector<double> GeneticAlgorithm::RANKING_PROBABILITY
    = {0.15 , 0.15, 0.15, 0.15, 0.1, 0.1, 0.1, 0.1};
std::size_t GeneticAlgorithm::TOURNAMENT_SIZE = 3;
std::size_t GeneticAlgorithm::NUM_ELITE = 2;
std::size_t GeneticAlgorithm::NUM_POINT = 3;
GeneticAlgorithm::CrossingTag GeneticAlgorithm::CROSSING_TAG = GeneticAlgorithm::CrossingTag::UNIFORM_CROSSING;
GeneticAlgorithm::SelectionTag GeneticAlgorithm::SELECTION_TAG = GeneticAlgorithm::SelectionTag::TOURNAMENT_SELECTION;

GeneticAlgorithm::GeneticAlgorithm()
    : mIOVec()
    , mIdx(0)
    , mGen(1)
    , mIsInit(false)
    , mJson(nullptr)
{
}

GeneticAlgorithm::~GeneticAlgorithm()
{
    if(mJson != nullptr)
        delete mJson;
}

void GeneticAlgorithm::initialize()
{
    mJson = new Json();

    std::cout
        << std::setprecision(std::numeric_limits<double>::max_digits10);

    if(NUM_ELEMENT >= 1)
        mIOVec.emplace_back(Input::INIT, Output::INIT);

    for(std::size_t i = 1; i < NUM_ELEMENT; i++)
        // mIOVec.emplace_back(generateRandom(), Output::INIT);
        mIOVec.emplace_back(Input::INIT, Output::INIT);
    
    mutateDuplicateElements(mIOVec);
    // mutateTop(mIOVec);
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
            nextIO.emplace_back(iter->second, Output::INIT);
    }

    for(std::size_t i = nextIO.size(); i < NUM_ELEMENT; i++)
    {
        double rand = Random::random();
        double prob = 0.0;
        if((prob += CROSSOVER_RATE) > rand)
            nextIO.emplace_back(generateCrossing(select(), select()), Output::INIT);
        else if((prob += MUTATION_RATE) > rand)
            nextIO.emplace_back(generateMutation(select()), Output::INIT);
        else
            nextIO.emplace_back(select(), Output::INIT);
    }

    mutateDuplicateElements(nextIO);
    // mutateTop(nextIO);

    mIOVec.swap(nextIO);
}

void GeneticAlgorithm::setInputToEvaluator()
{
    if(!mIsInit)
    {
        initialize();

        std::cout << "gen: " << mGen << "\n";
        for(std::size_t i = 0; i < mIOVec.size(); i++)
        {
            std::cout << "    " << i << ": "
                << mIOVec[i].first.str() << "\n";
        }

        std::cout << std::endl;

        mIsInit = true;
    }

    if(mIdx == NUM_ELEMENT)
    {
        mIdx = 0;
        mGen++;
        evolve();

        std::cout << "gen: " << mGen << "\n";
        for(std::size_t i = 0; i < mIOVec.size(); i++)
        {
            std::cout << "    " << i << ": "
                << mIOVec[i].first.str() << "\n";
        }

        std::cout << std::endl;
    }

    Evaluator::INPUT = mIOVec[mIdx].first;
    // Evaluator::INPUT = Input({5, 100, 1, 0, 80});
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

    return ret;
}

Input GeneticAlgorithm::generateRandom() const
{
    Input ret = Input({Random::random()
        , Random::random()
        , Random::random()
        , Random::random()
        , Random::random()});

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

    return ret;
}

Input GeneticAlgorithm::generateMutation(const Input &src) const
{
    Input ret = generateRandom();

    return ret;
}

void GeneticAlgorithm::mutateDuplicateElements(std::vector<std::pair<Input, Output>> &iovec)
{
    for(std::size_t i = 0; i < iovec.size(); i++)
    {
        for(std::size_t j = 0; j < i; j++)
        {
            if(iovec[j].first.array == iovec[i].first.array)
                iovec[i].first[static_cast<std::size_t>(Random::random() * static_cast<double>(Input::ARRAY_SIZE))] = Random::random();
        }
    }
}

void GeneticAlgorithm::mutateTop(std::vector<std::pair<Input, Output>> &iovec)
{
    std::vector<std::size_t> iota(static_cast<std::size_t>(Input::ARRAY_SIZE));
    std::iota(iota.begin(), iota.end(), 0);

    for(std::size_t i = 1; i < iovec.size(); i++)
    {
        iovec[i].first.array = iovec.front().first.array;

        std::shuffle(iota.begin(), iota.end(), Random::random.engine());
        std::size_t num = static_cast<std::size_t>(Random::random() * static_cast<double>(Input::ARRAY_SIZE - 1)) + 1;
        
        for(std::size_t j = 0; j < num; j++)
            iovec[i].first.array[iota[j]] = Random::random();
    }
}

}