#include "button.h"
#include "gameObject.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


Button::Button(GLFWwindow *window, GLfloat x, GLfloat y, GLfloat x_size, GLfloat y_size) {
    this->window = window;
    this->x = x;
    this->y = y;
    this->x_size = x_size;
    this->y_size = y_size;
    GLfloat vertices[] = {x, y,
                          x, y + y_size,
                          x + x_size, y + y_size,
                          x + x_size, y + y_size,
                          x + x_size, y,
                          x, y};


    // Set up vertex data (and buffer(s)) and attribute pointers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *) 0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0); // Unbind VAO (it's always a good thing to unbind any buffer/array to prevent strange bugs)

}

Button::~Button() {
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Button::render() {
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

bool Button::is_pointed() {
    double x_pos, y_pos;
    glfwGetCursorPos(window, &x_pos, &y_pos);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    x_pos = 2 * ((x_pos) / width - .5);
    y_pos = -2 * ((y_pos) / height - .5);
    if (x < x_pos && x_pos < x + x_size && y < y_pos && y_pos < y + y_size) {
        return true;
    }
    return false;
}

