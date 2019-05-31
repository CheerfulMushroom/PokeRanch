#ifndef PREP_POKEMON_RANCH_GAMEOBJECT_H
#define PREP_POKEMON_RANCH_GAMEOBJECT_H

#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"
#include "GameWindow.h"


class Game;

class GameState;


extern Game *game_object;

class Game {
public:
    Game(int width, int height, double rate);
    ~Game();

    // Запускает игровой цикл
    void start();
    void render_game();
    void update_game();
    // Меняет состояние игры
    void change_state(std::unique_ptr<GameState> new_state);

    // Геттеры
    GLFWwindow *get_window();
    GameState *get_state();
    int get_width();
    int get_height();

    int cam_id = 0;

private:
    double last_loop_time = 0;
    int width;
    int height;
    double rate;

    GameWindow screen;
    std::unique_ptr<GameState> state;
};

#endif //PREP_POKEMON_RANCH_GAMEOBJECT_H
