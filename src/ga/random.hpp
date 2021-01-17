#ifndef GA_RANDOM_HPP
#define GA_RANDOM_HPP

#include <limits>
#include <random>

namespace GA
{

class Random
{
public:
    static Random random;

    Random()
        : mEngine()
    {
        std::random_device seedGen;
        mEngine = std::mt19937_64(seedGen());
    }

    double operator ()()
    {
        return std::generate_canonical<double, std::numeric_limits<double>::digits>(mEngine);
    }

    std::mt19937_64 &engine()
        {return mEngine;}

private:
    std::mt19937_64 mEngine;
};

}

#endif