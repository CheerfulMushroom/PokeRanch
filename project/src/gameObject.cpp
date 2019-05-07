#include "gameObject.h"

#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>
#include "ShaderProgram.h"


GameObject::GameObject(GLuint WIDTH, GLuint HEIGHT) {
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(WIDTH, HEIGHT, "PokeRanch", nullptr, nullptr);
    glfwSetWindowUserPointer(window, this);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Define the viewport dimensions
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    buttonShader = ShaderProgram("project/shaders/button_v_shader.txt", "project/shaders/button_f_shader.txt");
    state = std::make_unique<MenuState>(this);
}

GameObject::~GameObject() {
    glfwTerminate();
}

void GameObject::start() {
// Game loop
    while (!glfwWindowShouldClose(window)) {
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
