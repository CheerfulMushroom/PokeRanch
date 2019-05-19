#ifndef PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H
#define PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H

/** Здесь храняться функции, которые записываются в кнопки
 *  Функции не должны принимать аргументов
 */

#include <GL/glew.h>

#include "Game.h"

template<class State>
void bf_change_game_state() {
    game_object->change_state(std::make_unique<State>(game_object));
}

double test_movement(double t);


#endif //PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H
