#include "Game.h"
#include "States.h"


static void mouse_button_callback(GLFWwindow *window, int mouse_button, int action, int mods);


Game::Game(int width, int height, double rate) {
    // Запись объекта игры в глобальную переменную
    game_object = this;

    screen = GameWindow(width, height);
    // Назначаем функцию, которая будет заниматься обработкой пользовательского ввода мышкой
    glfwSetMouseButtonCallback(get_window(), mouse_button_callback);

    change_state(std::make_unique<MenuState>(this));
}


Game::~Game() {
    glfwTerminate();
}


void Game::render_game() {
    state->render_game();
}


void Game::update_game() {
    state->update_game();
}


void Game::change_state(std::unique_ptr<GameState> new_state) {
    state = std::move(new_state);
    state->load_scene();
}


void Game::start() {
    GLFWwindow *window = get_window();

    while (!glfwWindowShouldClose(window)) {
        glfwPollEvents();
        render_game();
    }
}


GLFWwindow *Game::get_window() {
    return screen.get_window();
}



GameState *Game::get_state() {
    return state.get();
}


// Обработка пользовательского ввода (нажатие кнопки мыши)
static void mouse_button_callback(GLFWwindow *window, int mouse_button, int action, int mods) {
    if (mouse_button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        auto execs = &game_object->get_state()->to_exec;
        for (auto &exec_obj: *execs) {
            if (exec_obj->is_triggered()) {
                exec_obj->exec();
                break;
            }
        }
    }
}
