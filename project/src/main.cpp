#include <iostream>

#include "Game.h"
#include "States.h"

std::unique_ptr<Game> game_object;

int main() {
    game_object = std::make_unique<Game>(800,600, 25);
    game_object->start();
    return 0;
}
