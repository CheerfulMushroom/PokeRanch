#include <iostream>

#include "Game.h"

Game* game_object;

int main() {
    Game game(1280, 720, 60);
    game.start();
    return 0;
}
