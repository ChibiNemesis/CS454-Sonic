#include"Game.h"
#undef main

int main() {
    Game* game = new Game("Sonic The Hedghog", 480, 640);
    game->setfps(30); //probably useless
    game->setmovementspeed(DEFAULT_MOVEMENT_SPEED);

    while (game->getrunning()) {
        game->mainloop();
    }
    
    return 0;
}