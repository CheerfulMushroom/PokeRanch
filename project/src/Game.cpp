#include "Game.h"

static void mouse_button_callback(GLFWwindow *window, int mouse_button, int action, int mods);


Game::Game(int width, int height, double rate) {

    screen = GameWindow(width, height);
    glfwSetMouseButtonCallback(get_window(), mouse_button_callback);


    buttonShader = ShaderProgram("project/shaders/button_v_shader.txt", "project/shaders/button_f_shader.txt");
    state = std::make_unique<MenuState>(this);
}


Game::~Game() {
    glfwTerminate();
}


GLFWwindow* Game::get_window(){
    return screen.get_window();
}


ShaderProgram Game::get_shader_button() {
    return buttonShader;
}


GameState* Game::get_state() {
    return state.get();
}


void Game::start() {
// Game loop
    GLFWwindow *window = get_window();
    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        render_game();
    }
}


static void mouse_button_callback(GLFWwindow *window, int mouse_button, int action, int mods) {
    static int cursor_state = GLFW_RELEASE;
    if (mouse_button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        cursor_state = GLFW_PRESS;
    }
    if (mouse_button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        cursor_state = GLFW_PRESS;
        auto buttons = &game_object->get_state()->buttons;
        for (auto &button: *buttons) {
            if (button->is_pointed_at()) {
                button->exec();
                break;
            }
        }
    }
}
