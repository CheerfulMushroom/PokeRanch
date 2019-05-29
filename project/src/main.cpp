#include <iostream>

#include "Game.h"

Game* game_object;

int main() {
    Game game(400, 300, 25);
    game.start();
    return 0;
}
