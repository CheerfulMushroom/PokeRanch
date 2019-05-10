#ifndef PREP_POKEMON_RANCH_GAMEOBJECT_H
#define PREP_POKEMON_RANCH_GAMEOBJECT_H

#include "States.h"

#include <GL/glew.h>
#include "ShaderProgram.h"
#include "GameWindow.h"

extern std::unique_ptr<Game> game_object;

class Game {
public:
    Game(int width, int height, double rate);

    ~Game();

    void start();

    void render_game() { state->render_game(); }

    void update_game() { state->update_game(); }

    void change_state(std::unique_ptr<GameState> new_state) { state = std::move(new_state); }

    GLFWwindow *get_window();

    ShaderProgram get_shader_button();

    GameState *get_state();

private:
    GameWindow screen;
    ShaderProgram buttonShader;
    std::unique_ptr<GameState> state;


};

#endif //PREP_POKEMON_RANCH_GAMEOBJECT_H
