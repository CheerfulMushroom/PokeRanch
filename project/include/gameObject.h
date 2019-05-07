#ifndef PREP_POKEMON_RANCH_GAMEOBJECT_H
#define PREP_POKEMON_RANCH_GAMEOBJECT_H

#include "states.h"

#define GLEW_STATIC
#include <GL/glew.h>

class GameObject {
public:
    GameObject(GLuint WIDTH, GLuint HEIGHT);
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

    GLFWwindow* window;
    GLuint buttonShader;

private:
    std::unique_ptr<GameState> state;
    static void mouse_button_callback(GLFWwindow *window, int button, int action, int mods);
};

#endif //PREP_POKEMON_RANCH_GAMEOBJECT_H
