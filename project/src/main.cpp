#include <iostream>

#include "GameObject.h"
#include "States.h"

std::unique_ptr<GameObject> game_object;

int main() {
    game_object = std::make_unique<GameObject>(800,600, 25);
    game_object->start();
    return 0;
}
