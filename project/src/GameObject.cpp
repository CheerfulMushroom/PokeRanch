#include "GameObject.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include "ShaderProgram.h"
#include "GameWindow.h"


GameObject::GameObject(int width, int height, double rate) {

    screen = GameWindow(width, height, rate);
    glfwSetWindowUserPointer(screen.window, this);
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

void GameObject::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    static int cursor_state = GLFW_RELEASE;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        cursor_state = GLFW_PRESS;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        cursor_state = GLFW_PRESS;
        auto game = (GameObject*) glfwGetWindowUserPointer(window);
        for (auto& button_ptr: game->state->buttons) {
            if (button_ptr->is_pointed()){
                button_ptr->exec();
                std::cout << "Button was pressed"<<std::endl;
                break;
            }
        }
    }
}
