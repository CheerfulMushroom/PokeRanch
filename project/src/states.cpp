#include "states.h"
#include "gameObject.h"

MenuState::MenuState(GameObject *game_object) : GameState(game_object){}

void MenuState::render_game() {}

void MenuState::update_game() {}


PauseState::PauseState(GameObject *game_object) : GameState(game_object) {}

void PauseState::render_game() {}

void PauseState::update_game() {}


FarmState::FarmState(GameObject *game_object) : GameState(game_object) {}

void FarmState::render_game() {}

void FarmState::update_game() {}


PokedexState::PokedexState(GameObject *game_object) : GameState(game_object) {}

void PokedexState::render_game() {}

void PokedexState::update_game() {}

