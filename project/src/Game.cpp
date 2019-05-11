#include "Game.h"
#include "States.h"



static void mouse_button_callback(GLFWwindow *window, int mouse_button, int action, int mods);


Game::Game(int width, int height, double rate) {
    game_object = this;

    screen = GameWindow(width, height);
    glfwSetMouseButtonCallback(get_window(), mouse_button_callback);


    buttonShader = ShaderProgram("project/shaders/button_v_shader.txt", "project/shaders/button_f_shader.txt");
    state = std::make_unique<MenuState>(this);
}


Game::~Game() {
    glfwTerminate();
}

void Game::render_game() { state->render_game(); }

void Game::update_game() { state->update_game(); }

void Game::change_state(std::unique_ptr<GameState> new_state) { state = std::move(new_state); }

void Game::start() {
// Game loop
    GLFWwindow *window = get_window();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        render_game();
    }
}

GLFWwindow *Game::get_window() {
    return screen.get_window();
}


ShaderProgram Game::get_shader_button() {
    return buttonShader;
}


GameState *Game::get_state() {
    return state.get();
}


static void mouse_button_callback(GLFWwindow *window, int mouse_button, int action, int mods) {
    static int cursor_state = GLFW_RELEASE;
    if (mouse_button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        cursor_state = GLFW_PRESS;
    }
    if (mouse_button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        cursor_state = GLFW_PRESS;
        auto execs = &game_object->get_state()->to_exec;
        for (auto &exec_obj: *execs) {
            if (exec_obj->triggered()) {
                exec_obj->exec();
                break;
            }
        }
    }
}
