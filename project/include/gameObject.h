#ifndef PREP_POKEMON_RANCH_GAMEOBJECT_H
#define PREP_POKEMON_RANCH_GAMEOBJECT_H

#include "states.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include "ShaderProgram.h"
#include "GameWindow.h"

class GameObject {
public:
    GameObject(int width, int height, double rate);
    ~GameObject();
    void start();
    void render_game() {
        state->render_game();
    }

    void update_game() {
        state->update_game();
    }

    void change_state(std::unique_ptr<GameState> state) {
        this->state = std::move(state);
    }

    GameWindow screen;
    ShaderProgram buttonShader;

    std::unique_ptr<GameState> state;
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
};

#endif //PREP_POKEMON_RANCH_GAMEOBJECT_H
