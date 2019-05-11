#include "States.h"
#include "Button.h"
#include "ButtonFunctions.h"
#include "Interfaces.h"

Game* GameState::get_game() {
    return game;
}

MenuState::MenuState(Game *game_object) : GameState(game_object) {}

void MenuState::load_scene() {
    game_elements.emplace_back(std::make_unique<Button>(this, 0.5f, 0.5f, 0.3f, 0.3f, bf_change_game_state<RanchState>));
//    game_elements.emplace_back(std::make_unique<Button>(this, -0.5f, -0.5f, 0.3f, 0.3f, bf_change_game_state<PauseState>));
}

void MenuState::render_game() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto &render_obj: to_render) {
        render_obj->render();
    }
    glfwSwapBuffers(game->get_window());
}

void MenuState::update_game() {}




PauseState::PauseState(Game *game_object) : GameState(game_object) {}

void PauseState::load_scene() {
    game_elements.emplace_back(std::make_unique<Button>(this, 0.5f, -0.5f, 0.3f, 0.3f, bf_change_game_state<RanchState>));
//    buttons.emplace_back(std::make_unique<Button>(this, -0.5f, 0.5f, 0.3f, 0.3f, bf_change_game_state<MenuState>));
}

void PauseState::render_game() {
    glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto &render_obj: to_render) {
        render_obj->render();
    }
    glfwSwapBuffers(game->get_window());
}

void PauseState::update_game() {}



//TODO(me): add cam_index
RanchState::RanchState(Game *game_object) : GameState(game_object), video_stream(VideoStream(0)) {
}

void RanchState::load_scene() {
    game_elements.emplace_back(std::make_unique<Button>(this, 0.6f, 0.6f, 0.3f, 0.3f, bf_change_game_state<PauseState>));
}

void RanchState::render_game() {
    glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    video_stream.render();

    for (auto &render_obj: to_render) {
        render_obj->render();
    }
    glfwSwapBuffers(game->get_window());
}

void RanchState::update_game() {}




PokedexState::PokedexState(Game *game_object) : GameState(game_object) {
}
void PokedexState::load_scene() {
    game_elements.emplace_back(std::make_unique<Button>(this, 0.5f, 0.5f, 0.3f, 0.3f, bf_change_game_state<PauseState>));
    game_elements.emplace_back(std::make_unique<Button>(this, -0.5f, -0.5f, 0.3f, 0.3f, bf_change_game_state<MenuState>));

}

void PokedexState::render_game() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    for (auto &render_obj: to_render) {
        render_obj->render();
    }
    glfwSwapBuffers(game->get_window());
}

void PokedexState::update_game() {}

