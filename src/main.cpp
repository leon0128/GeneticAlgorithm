#include <iostream>
#include "ga/genetic_algorithm.hpp"
#include "game.hpp"
int main(int argc, char** argv)
{
    GA::OUTPUT_FILENAME = argc >= 2 ? argv[1] : "";
    Game game;
    if(game.initialize())
    {
        game.runLoop();
    }
    game.finalize();
    return 0;
}