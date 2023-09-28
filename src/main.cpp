#include "../include/game_engine.h"

int main()
{
    std::cout << "Setting up application..." << '\n';
    GameEngine engine;
    engine.startGameLoop();

    return 0;
}
