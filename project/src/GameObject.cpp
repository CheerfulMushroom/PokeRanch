#include "GameObject.h"

#include <GL/glew.h>

#include <iostream>
#include "ShaderProgram.h"
#include "GameWindow.h"

static void mouse_button_callback(GLFWwindow *window, int mouse_button, int action, int mods);


GameObject::GameObject(int width, int height, double rate) {

    screen = GameWindow(width, height, rate);
    glfwSetMouseButtonCallback(screen.window, mouse_button_callback);


    buttonShader = ShaderProgram("project/shaders/button_v_shader.txt", "project/shaders/button_f_shader.txt");
    state = std::make_unique<MenuState>(this);
}


GameObject::~GameObject() {
    glfwTerminate();
}


void GameObject::start() {
// Game loop
    while (!glfwWindowShouldClose(screen.window)) {
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
        auto buttons = &game_object->state->buttons;
        for (auto& button: *buttons) {
            if (button->is_pointed()){
                button->exec();
                std::cout << "Button was pressed"<<std::endl;
                break;
            }
        }
    }
}
