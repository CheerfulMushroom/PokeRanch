#ifndef PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H
#define PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H

#include "States.h"
#include "GameObject.h"

template<class State>
void bf_change_game_state() {
    game_object->change_state(std::make_unique<State>(game_object.get()));
}


#endif //PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H
