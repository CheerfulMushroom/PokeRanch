#include <iostream>

#include "states.h"
#include "gameObject.h"


int main() {
    auto game_object = std::make_unique<GameObject>(800,600);

    game_object->start();
    return 0;
}
