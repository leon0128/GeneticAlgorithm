#ifndef GA_JSON_HPP
#define GA_JSON_HPP

#include <string>

#include <boost/property_tree/ptree.hpp>

namespace GA
{

class Input;
class Output;

class Json
{
private:
    static const std::string PROPERTY_FILENAME;

public:
    Json();
    ~Json();

    void init();
    void fini();

    void add(std::size_t gen
        , std::size_t idx
        , const Input&
        , const Output&);

private:
    bool mIsInitialized;

    boost::property_tree::ptree mPtree;
    boost::property_tree::ptree mChild;
};

}

#endif