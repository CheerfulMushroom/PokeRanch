#ifndef PREP_POKEMON_RANCH_GAMEOBJECT_H
#define PREP_POKEMON_RANCH_GAMEOBJECT_H

#include "states.h"
#include "button.h"

class GameObject {
public:
    void render_game() {
        state->render_game();
    }

    void update_game() {
        state->update_game();
    }

    void change_state(std::unique_ptr<GameState> state) {
        this->state = std::move(state);
    }

private:
    std::unique_ptr<GameState> state = std::make_unique<MenuState>(this);
};

#endif //PREP_POKEMON_RANCH_GAMEOBJECT_H
