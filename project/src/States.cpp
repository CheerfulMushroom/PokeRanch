#include "States.h"
#include "Game.h"
#include "ButtonFunctions.h"

Game* GameState::get_game() {
    return game;
}

MenuState::MenuState(Game *game_object) : GameState(game_object) {
    buttons.emplace_back(std::make_unique<Button>(this, 0.5f, 0.5f, 0.3f, 0.3f, bf_change_game_state<PauseState>));
    buttons.emplace_back(std::make_unique<Button>(this, -0.5f, -0.5f, 0.3f, 0.3f, bf_change_game_state<PauseState>));
}

void MenuState::render_game() {
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto &button: buttons) {
        button->render();
    }
    glfwSwapBuffers(game->get_window());
}

void MenuState::update_game() {}


PauseState::PauseState(Game *game_object) : GameState(game_object) {
    buttons.emplace_back(std::make_unique<Button>(this, 0.5f, -0.5f, 0.3f, 0.3f, bf_change_game_state<MenuState>));
    buttons.emplace_back(std::make_unique<Button>(this, -0.5f, 0.5f, 0.3f, 0.3f, bf_change_game_state<MenuState>));
}

void PauseState::render_game() {
    // Render
    // Clear the colorbuffer
    glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto &button: buttons) {
        button->render();
    }
    glfwSwapBuffers(game->get_window());
}

void PauseState::update_game() {}


FarmState::FarmState(Game *game_object) : GameState(game_object) {}

void FarmState::render_game() {}

void FarmState::update_game() {}


PokedexState::PokedexState(Game *game_object) : GameState(game_object) {}

void PokedexState::render_game() {}

void PokedexState::update_game() {}

