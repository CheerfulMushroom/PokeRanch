#include <glm/glm.hpp>

#include "States.h"
#include "Picture.h"
#include "Button.h"
#include "MovingPicture.h"
#include "AdditionalFunctions.h"
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
    add_element(std::make_unique<Picture>(-0.4f, 0.0f, 0.8f, 0.8f * 16 / 9, "project/pictures/pokemon_logo.png"));
    add_element(std::make_unique<Picture>(-0.25f, 0.25f, 0.5f, 0.5f * 16 / 9 * 56 / 288, "project/pictures/ranch.png"));
    add_element(std::make_unique<MovingPicture>(0.8f, 0.8f, 0.2f, 0.2f * 16 / 9, test_movement, test_movement, "project/icons/pikachu.png"));
    add_element(std::make_unique<Button>(this, -0.1f, -0.925f, 0.2f, 0.2f * 16 / 9, bf_change_game_state<RanchState>,
                                         "project/icons/play_pink.png"));
}

void MenuState::render_game() {
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto &render_obj: to_render) {
        render_obj->render();
    }
    glfwSwapBuffers(game->get_window());
}

void MenuState::update_game() {
    for (auto &update_obj: to_update) {
        update_obj->update();
    }
}


PauseState::PauseState(Game *game_object) : GameState(game_object) {}

void PauseState::load_scene() {
    add_element(std::make_unique<Picture>(-1.0f, -1.0f, 2.0f, 2.0f, "project/pictures/pause_background.png"));
    add_element(std::make_unique<Picture>(-0.4f, 0.0f, 0.8f, 0.8f * 16 / 9 * 127/330, "project/pictures/pause_logo.png"));
    add_element(std::make_unique<Button>(this, -0.1f, -0.925f, 0.2f, 0.2f * 16 / 9, bf_change_game_state<RanchState>,
                                         "project/icons/play_pink.png"));
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
                                            camera(glm::vec3(0.5f, 0.0f, 4.0f)) {}

void RanchState::load_scene() {
    add_element(std::make_unique<VideoStream>(0));

    add_element(std::make_unique<Model>("project/models/Pikachu/pm0025_00_fi.dae", &camera,
                                        glm::vec3(2.0f, -0.4f, 0.0f), glm::vec3(0.02, 0.02, 0.02),
                                        glm::vec3(0.0f, 1.0f, 0.0f), 45.0f));

    add_element(std::make_unique<Model>("project/models/Rockruff/dog.dae", &camera,
                                        glm::vec3(-0.8f, -0.4f, 0.0f), glm::vec3(0.02, 0.02, 0.02),
                                        glm::vec3(1.0f, 0.0f, 0.0f), 0));

    add_element(std::make_unique<Model>("project/models/Slowpoke/Slowpoke.dae", &camera,
                                        glm::vec3(1.2f, -0.4f, 0.0f), glm::vec3(0.01, 0.01, 0.01),
                                        glm::vec3(1.0f, 0.0f, 0.0f), 0));

    add_element(std::make_unique<Model>("project/models/Litten/pm0844_00_fi.dae", &camera,
                                        glm::vec3(0.2f, -0.4f, 0.0f), glm::vec3(0.03, 0.03, 0.03),
                                        glm::vec3(0.0f, 1.0f, 0.0f), 0));


    add_element(std::make_unique<Button>(this, -0.7f, -0.9f, 0.2f, 0.2f* 16/9, bf_change_game_state<PauseState>,
                                         "project/icons/menu_pink.png"));
    add_element(std::make_unique<Button>(this, 0.7f, -0.9f, 0.2f, 0.2f * 16 / 9, bf_change_game_state<PokedexState>,
                                         "project/icons/pikachu.png"));
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
    add_element(std::make_unique<Button>(this, -0.1f, -0.925f, 0.2f, 0.2f * 16 / 9, bf_change_game_state<RanchState>,
                                         "project/icons/play_pink.png"));

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

