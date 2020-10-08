#include <iostream>
#include <utility>

#include <boost/optional.hpp>
#include <boost/foreach.hpp>
#include <boost/property_tree/json_parser.hpp>

#include "genetic_algorithm.hpp"
#include "evaluator.hpp"
#include "input.hpp"
#include "output.hpp"
#include "json.hpp"

namespace GA
{

Json::Json()
    : mIsInitialized(false)
    , mPtree()
    , mChild()
{
}

Json::~Json()
{
    fini();
}

void Json::init()
{
    mPtree.put("parameter.num_element", GeneticAlgorithm::NUM_ELEMENT);
    mPtree.put("parameter.num_generation", GeneticAlgorithm::NUM_GENERATION);
    mPtree.put("parameter.crossing_tag", static_cast<int>(GeneticAlgorithm::CROSSING_TAG));
    mPtree.put("parameter.selection_tag", static_cast<int>(GeneticAlgorithm::SELECTION_TAG));
    mPtree.put("parameter.is_selected_elite", GeneticAlgorithm::IS_SELECTED_ELITE);
    mPtree.put("parameter.num_elite", GeneticAlgorithm::NUM_ELITE);
    mPtree.put("parameter.crossover_rate", GeneticAlgorithm::CROSSOVER_RATE);
    mPtree.put("parameter.mutation_rate", GeneticAlgorithm::MUTATION_RATE);
    boost::property_tree::ptree arr;
    for(auto &&d : GeneticAlgorithm::RANKING_PROBABILITY)
    {
        boost::property_tree::ptree e;
        e.put("", d);
        arr.push_back(std::make_pair("", e));
    }
    mPtree.add_child("parameter.ranking_probability", arr);
    mPtree.put("parameter.tournament_size", GeneticAlgorithm::TOURNAMENT_SIZE);
    mPtree.put("parameter.num_point", GeneticAlgorithm::NUM_POINT);
}

void Json::fini()
{
    mPtree.add_child("result", mChild);

    if(!GA::OUTPUT_FILENAME.empty())
        boost::property_tree::write_json(GA::OUTPUT_FILENAME, mPtree);
}

void Json::add(std::size_t gen
    , std::size_t idx
    , const Input &in
    , const Output &out)
{
    if(!mIsInitialized)
    {
        init();
        mIsInitialized = true;
    }

    boost::property_tree::ptree node;

    node.put("gen", gen);
    node.put("idx", idx);
    node.put("score", Evaluator::score(out));
    {
        boost::property_tree::ptree arr;
        for(auto &&i : in.array)
        {
            boost::property_tree::ptree e;
            e.put("", i);
            arr.push_back(std::make_pair("", e));
        }
        node.add_child("input", arr);
    }
    {
        boost::property_tree::ptree arr;
        for(auto &&i : out.array)
        {
            boost::property_tree::ptree e;
            e.put("", i);
            arr.push_back(std::make_pair("", e));
        }
        node.add_child("output", arr);
    }

    mChild.push_back(std::make_pair("", node));

    std::cout << "=== gen: " << gen
        << ", idx: " << idx
        << " ===\n"
        "score: " << Evaluator::score(out)
        << "\n"
        "input: [";
    for(auto &&i : in.array)
        std::cout << i << ", ";
    std::cout << "]\n"
        "output: [";
    for(auto &&i : out.array)
        std::cout << i << ", ";
    std::cout << "]\n\n"
        << std::flush;
}

}