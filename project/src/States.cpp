#include <glm/glm.hpp>

#include "States.h"
#include "Picture.h"
#include "Button.h"
#include "ButtonFunctions.h"
#include "Model.h"


void GameState::add_element(std::unique_ptr<GameElement> el) {
    game_elements.emplace_back(std::move(el));
}


Game *GameState::get_game() {
    return game;
}





MenuState::MenuState(Game *game_object) : GameState(game_object) {}

void MenuState::load_scene() {
    add_element(std::make_unique<Picture>(-1.0f, -1.0f, 2.0f, 2.0f, "project/pictures/main_screen.png"));
    add_element(std::make_unique<Button>(this, -0.0f, -0.85f, 0.2f, 0.2f, bf_change_game_state<RanchState>, "play_pink.png"));
}

void MenuState::render_game() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &render_obj: to_render) {
        render_obj->render();
    }
    glfwSwapBuffers(game->get_window());
}

void MenuState::update_game() {}





PauseState::PauseState(Game *game_object) : GameState(game_object) {}

void PauseState::load_scene() {
    add_element(std::make_unique<Button>(this, 0.5f, -0.5f, 0.1f, 0.1f, bf_change_game_state<RanchState>));
    add_element(std::make_unique<Button>(this, -0.5f, +0.5f, 0.1f, 0.1f, bf_change_game_state<RanchState>));
}

void PauseState::render_game() {
    glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &render_obj: to_render) {
        render_obj->render();
    }
    glfwSwapBuffers(game->get_window());
}

void PauseState::update_game() {}





//TODO(me): add cam_index
RanchState::RanchState(Game *game_object) : GameState(game_object),
                                            camera(glm::vec3(0.0f, 0.0f, 5.0f)) {}

void RanchState::load_scene() {
    add_element(std::make_unique<VideoStream>(0));
    add_element(std::make_unique<Model>("project/models/pikachu.dae", &camera,
            glm::vec3(0.5f, -0.4f, 0.0f), glm::vec3(0.05, 0.05, 0.05), glm::vec3(1.0f, 0.0f, 0.0f)));

    add_element(std::make_unique<Button>(this, 0.6f, 0.6f, 0.3f, 0.3f, bf_change_game_state<PauseState>));
    add_element(std::make_unique<Button>(this, 0.6f, 0.6f, 0.3f, 0.3f, bf_change_game_state<PokedexState>));
}

void RanchState::render_game() {
    glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    for (auto &render_obj: to_render) {
        render_obj->render();
    }
    glfwSwapBuffers(game->get_window());
}

void RanchState::update_game() {}





PokedexState::PokedexState(Game *game_object) : GameState(game_object) {
}

void PokedexState::load_scene() {
    add_element(std::make_unique<Button>(this, 0.5f, 0.5f, 0.3f, 0.3f, bf_change_game_state<PauseState>));
    add_element(std::make_unique<Button>(this, -0.5f, -0.5f, 0.3f, 0.3f, bf_change_game_state<MenuState>));

}

void PokedexState::render_game() {
    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &render_obj: to_render) {
        render_obj->render();
    }
    glfwSwapBuffers(game->get_window());
}

void PokedexState::update_game() {}

