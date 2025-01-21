#include"Game.h"
#undef main

int main() {
    std::cout << "Game not created\n";
    Game* game = new Game("Sonic The Hedghog", 256, 256);
    game->setfps(30); //probably useless
    game->setmovementspeed(15);

    while (game->getrunning()) {
        game->mainloop();
    }

    return 0;
}