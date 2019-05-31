#include "ButtonFunctions.h"

void change_to_ranch() {
    game_object->change_state(std::make_unique<RanchState>(game_object));
}

void change_to_pokedex(int pokemon_id) {
    game_object->change_state(std::make_unique<PokedexState>(game_object, pokemon_id));
}

void change_to_pause() {
    game_object->change_state(std::make_unique<PauseState>(game_object));
}

void swap_cam(){
    game_object->cam_id = (game_object->cam_id + 1) % 3;
    game_object->change_state(std::make_unique<RanchState>(game_object));
}