#include "Interfaces.h"
#include "States.h"

Renderable::Renderable() {
    game_object->get_state()->to_render.emplace_back(this);
}

Interactable::Interactable() {
    game_object->get_state()->to_exec.emplace_back(this);
}

Updatable::Updatable() {
    game_object->get_state()->to_update.emplace_back(this);
}
