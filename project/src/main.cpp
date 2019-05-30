#include <iostream>

#include "Game.h"

Game* game_object;

int main() {
    Game game(1200, 600, 25);
    game.start();
    return 0;
}
