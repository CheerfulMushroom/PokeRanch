#include <iostream>

#include "states.h"
#include "gameObject.h"


int main() {
    auto game_object = std::make_unique<GameObject>(800,600);

    game_object->start();
//    while (true) {
//        int state_num;
//        std::cin >> state_num;
//        switch (state_num) {
//            case 0:
//                game_object.change_state(std::make_unique<MenuState>(&game_object));
//                break;
//            case 1:
//                game_object.change_state(std::make_unique<FarmState>(&game_object));
//                break;
//            case 2:
//                game_object.change_state(std::make_unique<PauseState>(&game_object));
//                break;
//            case 3:
//                game_object.change_state(std::make_unique<PokedexState>(&game_object));
//                break;
//        }
//        if (state_num == 4) {
//            break;
//        }
//
//    }
//    return 0;
}
