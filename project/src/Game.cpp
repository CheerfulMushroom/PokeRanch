#include "Game.h"
#include "States.h"


static void mouse_button_callback(GLFWwindow *window, int mouse_button, int action, int mods);


Game::Game(int width, int height, double rate) {
    this->width = width;
    this->height = height;
    this->rate = rate;


    // Запись объекта игры в глобальную переменную
    game_object = this;

    screen = GameWindow(width, height);

    // Назначаем функцию, которая будет заниматься обработкой пользовательского ввода мышкой
    glfwSetMouseButtonCallback(get_window(), mouse_button_callback);

    change_state(std::make_unique<RanchState>(this));
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

//TODO(alexandr): сделать нормалный игровой цикл
void Game::start() {
    GLFWwindow *window = get_window();

    while (!glfwWindowShouldClose(window)) {
        if (glfwGetTime()- last_loop_time > 1/rate) {
            last_loop_time = glfwGetTime();
            glfwPollEvents();
            update_game();
            render_game();
        }
    }
}


GLFWwindow *Game::get_window() {
    return screen.get_window();
}


GameState *Game::get_state() {
    return state.get();
}

int Game::get_width() {
    return width;
}

int Game::get_height() {
    return height;
}


static bool
compare_values(const std::pair<Interactable *, double> &left, const std::pair<Interactable *, double> &right) {
    return left.second < right.second;
}

// Обработка пользовательского ввода (нажатие кнопки мыши)
static void mouse_button_callback(GLFWwindow *window, int mouse_button, int action, int mods) {
    if (mouse_button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        auto execs = &game_object->get_state()->to_exec;
        std::map<Interactable *, double> contenders;

        for (auto &exec_obj: *execs) {
            if (exec_obj->is_pointed_at()) {
                contenders.insert({exec_obj, exec_obj->get_distance()});
            }
        }

        if (!contenders.empty()) {
            std::pair<Interactable *, double> min = *min_element(contenders.begin(), contenders.end(), compare_values);
            min.first->exec();
        }

    }
}

