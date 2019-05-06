#include "gameObject.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


GameObject::GameObject(GLuint WIDTH, GLuint HEIGHT) {
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "PokeRanch", nullptr, nullptr);
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
}

void GameObject::mouse_button_callback(GLFWwindow *window, int button, int action, int mods) {
    static int state = GLFW_RELEASE;
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        state = GLFW_PRESS;
    }
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE) {
        state = GLFW_PRESS;
        for (auto button: this->state) {
            if (button->is_pointed()) {
                glfwSetWindowShouldClose(window, GL_TRUE);
            }
        }
    }
}