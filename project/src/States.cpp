#include "States.h"
#include "GameObject.h"
#include "Buttons.h"

MenuState::MenuState(GameObject *game_object) : GameState(game_object) {
    buttons.emplace_back(std::make_unique<Buttons<PauseState>>(this, 0.5f, 0.5f, 0.3f, 0.3f));
    buttons.emplace_back(std::make_unique<Buttons<PauseState>>(this, -0.5f, -0.5f, 0.3f, 0.3f));

}

void MenuState::render_game() {
    // Render
    // Clear the colorbuffer
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& button: buttons) {
        button->render();
    }
    glfwSwapBuffers(game->screen.window);
}

void MenuState::update_game() {}


PauseState::PauseState(GameObject *game_object) : GameState(game_object) {
    buttons.emplace_back(std::make_unique<Buttons<MenuState>>(this, 0.5f, -0.5f, 0.3f, 0.3f));
    buttons.emplace_back(std::make_unique<Buttons<MenuState>>(this, 0.5f, -0.5f, 0.3f, 0.3f));
}

void PauseState::render_game() {
    // Render
    // Clear the colorbuffer
    glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto& button: buttons) {
        button->render();
    }
    glfwSwapBuffers(game->screen.window);
}

void PauseState::update_game() {}


FarmState::FarmState(GameObject *game_object) : GameState(game_object) {}

void FarmState::render_game() {}

void FarmState::update_game() {}


PokedexState::PokedexState(GameObject *game_object) : GameState(game_object) {}

void PokedexState::render_game() {}

void PokedexState::update_game() {}

