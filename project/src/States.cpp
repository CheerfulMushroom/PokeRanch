#include <glm/glm.hpp>

#include "States.h"
#include "Picture.h"
#include "Button.h"
#include "ButtonFunctions.h"
#include "Model.h"
#include "AnimModel.h"
#include "Cake.h"


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
    add_element(
            std::make_unique<Button>(this, -0.1f, -0.925f, 0.2f, 0.2f * 16 / 9, change_to_ranch,
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
    add_element(
            std::make_unique<Picture>(-0.4f, 0.0f, 0.8f, 0.8f * 16 / 9 * 127 / 330, "project/pictures/pause_logo.png"));
    add_element(std::make_unique<Button>(this, -0.1f, -0.925f, 0.2f, 0.2f * 16 / 9, change_to_ranch,
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

void PauseState::update_game() {
    for (auto &update_obj: to_update) {
        update_obj->update();
    }
}


//TODO(me): add cam_index
RanchState::RanchState(Game *game_object) : GameState(game_object) {};

void RanchState::load_scene() {
    int width = game_object->get_width();
    int height = game_object->get_height();


    auto stream = std::make_unique<VideoStream>(1);
    auto marker_detector = std::make_unique<MarkerDetector>(stream.get(),
                                                            "project/microsoft_webcam_calibration.yml",
                                                            width, height,
                                                            this);
    add_element(std::move(marker_detector));
    add_element(std::move(stream));


    add_element(std::make_unique<Button>(this, -0.7f, -0.9f, 0.2f, 0.2f * 16 / 9, change_to_pause,
                                         "project/icons/menu_pink.png"));

}

void RanchState::render_game() {
    glClearColor(0.5f, 0.3f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    for (auto &render_obj: to_render) {
        render_obj->render();
    }
    glfwSwapBuffers(game->get_window());
}

void RanchState::update_game() {
    // TODO(leb): ПОКАЗАТЬ ПРЕПОДАМ
    for (size_t i = 0; i < to_update.size(); i++) {
        auto update_obj = to_update[i];
        update_obj->update();
    }
}


PokedexState::PokedexState(Game *game_object, int pokemon_id) : GameState(game_object),
                                                                camera(glm::vec3(0.0f, 0.0f, 4.0f)),
                                                                pokemon_id(pokemon_id) {
}

void PokedexState::load_scene() {
    int width = game_object->get_width();
    int height = game_object->get_height();


    add_element(std::make_unique<Picture>(-1.0f, -1.0f, 2.0f, 2.0f, "project/pictures/pokedex_background.jpg"));


    auto model = std::make_unique<AnimModel>(pokemon_id,
                                             &camera,
                                             glm::vec3(.0f, -0.5f, .0f),
                                             glm::vec3(0.02, 0.02, 0.02),
                                             glm::vec3(0.0f, 1.0f, 0.0f),
                                             180.0f,
                                             width,
                                             height);

    add_element(std::make_unique<Button>(this, -0.5f, -0.925f, 0.2f, 0.2f * 16 / 9,
                                         std::bind(&AnimModel::rotate, model.get(), 15),
                                         "project/icons/nonclockwise.png"));
    add_element(std::make_unique<Button>(this, 0.5f, -0.925f, 0.2f, 0.2f * 16 / 9,
                                         std::bind(&AnimModel::rotate, model.get(), -15),
                                         "project/icons/clockwise.png"));
    add_element(std::make_unique<Button>(this, 0.5f, -.3, 0.2f, 0.2f * 16 / 9,
                                         std::bind(&AnimModel::change_animation, model.get(),
                                                   "project/models/Pikachu/pikachu_run.dae")));

    add_element(std::move(model));

    add_element(std::make_unique<Cake>("project/models/Autumn/okashi_special_04_02.dae",
                                       &camera,
                                       glm::vec3(1.2f, -0.4f, 0.0f),
                                       glm::vec3(0.04, 0.04, 0.04),
                                       glm::vec3(1.0f, 0.0f, 0.0f),
                                       0));


    add_element(
            std::make_unique<Button>(this, -0.1f, -0.925f, 0.2f, 0.2f * 16 / 9, change_to_ranch,
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

void PokedexState::update_game() {
    for (auto &update_obj: to_update) {
        update_obj->update();
    }
}

