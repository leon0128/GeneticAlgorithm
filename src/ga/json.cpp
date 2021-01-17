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
    : mPtree()
    , mChild()
{
    init();
}

Json::~Json()
{
    fini();
}

void Json::init()
{
    bool isOpen = true;
    
    boost::property_tree::ptree ptree;
    try
    {
        boost::property_tree::read_json(PROPERTY_FILENAME, ptree);
    }
    catch(const std::exception &e)
    {
        isOpen = false;
        std::cerr << e.what() << std::endl;
    }
    if(isOpen)
    {
        Evaluator::NPC_DEPTH = ptree.get_optional<int>("game_property.npc_search_depth").get();
        Evaluator::MAX_NUM_USED_BLOCK = ptree.get_optional<int>("game_property.num_max_used_block").get();
        Evaluator::USED_BLOCK_MULTIPLE = ptree.get_optional<int>("game_property.used_block_score_multiple").get();
        Evaluator::TETRIS_SCORE_MULTIPLE = ptree.get_optional<int>("game_property.tetris_score_multiple").get();
        Evaluator::THREE_LINES_SCORE_MULTIPLE = ptree.get_optional<int>("game_property.three_lines_score_multiple").get();
        Evaluator::TWO_LINES_SCORE_MULTIPLE = ptree.get_optional<int>("game_property.two_lines_score_multiple").get();
        Evaluator::ONE_LINE_SCORE_MULTIPLE = ptree.get_optional<int>("game_property.one_line_score_multiple").get();

        GeneticAlgorithm::CROSSOVER_RATE = ptree.get_optional<double>("genetic_algorithm_property.crossover_rate").get();
        GeneticAlgorithm::MUTATION_RATE = ptree.get_optional<double>("genetic_algorithm_property.mutation_rate").get();
        GeneticAlgorithm::NUM_ELEMENT = ptree.get_optional<int>("genetic_algorithm_property.num_array_elements").get();
        GeneticAlgorithm::NUM_GENERATION = ptree.get_optional<int>("genetic_algorithm_property.num_generation").get();
        GeneticAlgorithm::IS_SELECTED_ELITE = ptree.get_optional<bool>("genetic_algorithm_property.is_selected_elite").get();
        GeneticAlgorithm::NUM_ELITE = ptree.get_optional<int>("genetic_algorithm_property.num_selected_elite").get();

        for(auto &&c : ptree.get_child("genetic_algorithm_property.ranking_probability"))
            GeneticAlgorithm::RANKING_PROBABILITY.push_back(std::stod(c.second.data()));

        GeneticAlgorithm::TOURNAMENT_SIZE = ptree.get_optional<int>("genetic_algorithm_property.tournament_size").get();
        GeneticAlgorithm::NUM_POINT = ptree.get_optional<int>("genetic_algorithm_property.num_multi_point").get();
        GeneticAlgorithm::CROSSING_TAG = static_cast<GeneticAlgorithm::CrossingTag>(ptree.get_optional<int>("genetic_algorithm_property.crossing_tag").get());
        GeneticAlgorithm::SELECTION_TAG = static_cast<GeneticAlgorithm::SelectionTag>(ptree.get_optional<int>("genetic_algorithm_property.selection_tag").get());

        std::size_t i = 0;
        for(auto &&c : ptree.get_child("init_value"))
        {
            Input::INIT[i] = std::stod(c.second.data());
            i++;
        }
    }

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

    try
    {
        boost::property_tree::write_json(GA::OUTPUT_FILENAME, mPtree);
    }
    catch(const std::exception &e)
    {
        std::cerr << e.what() << std::endl;
    }
}

void Json::add(std::size_t gen
    , std::size_t idx
    , const Input &in
    , const Output &out)
{
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

    std::cout << "gen: " << gen << ", idx: " << idx << "\n"
        "    score: " << Evaluator::score(out) << "\n"
        "    " << in.str() << "\n"
        "    " << out.str()
        << std::endl;
}

}