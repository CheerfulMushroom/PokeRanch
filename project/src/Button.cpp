#include <functional>

#include "Button.h"
#include "States.h"
#include "Game.h"


Button::Button(GameState *state, GLfloat x, GLfloat y, GLfloat x_size, GLfloat y_size, std::function<void()> to_exec) {
    this->state = state;
    this->x = x;
    this->y = y;
    this->x_size = x_size;
    this->y_size = y_size;
    this->to_exec = std::move(to_exec);
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

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(GLfloat), (GLvoid *) (0 * sizeof(GLfloat)));
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
    state->get_game()->get_shader_button().use();
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 6);
    glBindVertexArray(0);
}

bool Button::is_pointed_at() {
    GLFWwindow *window = state->get_game()->get_window();

    double x_cursor, y_cursor;
    glfwGetCursorPos(window, &x_cursor, &y_cursor);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    x_cursor = 2 * ((x_cursor) / width - .5);
    y_cursor = -2 * ((y_cursor) / height - .5);
    if (x <= x_cursor && x_cursor <= x + x_size && y <= y_cursor && y_cursor <= y + y_size) {
        return true;
    }
    return false;
}

void Button::exec() {
    to_exec();
}
