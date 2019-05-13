#include <functional>

#include "Button.h"
#include "States.h"


Button::Button(GameState *state,
               GLfloat x, GLfloat y,
               GLfloat x_size, GLfloat y_size,
               std::function<void()> to_exec,
               std::string path_to_icon)
        : Renderable(), Interactable() {
    this->state = state;
    this->x = x;
    this->y = y;
    this->x_size = x_size;
    this->y_size = y_size;
    this->to_exec = std::move(to_exec);
    icon = cv::imread(path_to_icon, -1);

    shader = ShaderProgram("project/shaders/button_v_shader.txt", "project/shaders/button_f_shader.txt");

    GLfloat vertices[] = {
            x + x_size, y + y_size, 0.0f, 1.0f, 1.0f,
            x + x_size, y,          0.0f, 1.0f, 0.0f,
            x,          y,          0.0f, 0.0f, 0.0f,
            x,          y + y_size, 0.0f, 0.0f, 1.0f
    };

    GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
    };


    glGenTextures(1, &texture);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader.get_program(), "ourTexture1"), 0);


    glBindVertexArray(0);

    mat_to_texture();

}

Button::~Button() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
}

void Button::render() {
    shader.use();
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

bool Button::is_triggered() {
    GLFWwindow *window = state->get_game()->get_window();

    double x_cursor, y_cursor;
    glfwGetCursorPos(window, &x_cursor, &y_cursor);

    int width, height;
    glfwGetFramebufferSize(window, &width, &height);

    // Перевод из координат GLFW ([0;-1]) в координаты openGL ([-1;1])
    x_cursor = 2 * ((x_cursor) / width - .5);
    y_cursor = -2 * ((y_cursor) / height - .5);
    return x <= x_cursor && x_cursor <= x + x_size && y <= y_cursor && y_cursor <= y + y_size;

}

void Button::exec() {
    to_exec();
}

void Button::mat_to_texture() {
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    cv::flip(icon, icon, 0);


    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, icon.cols, icon.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, icon.ptr());

    glBindTexture(GL_TEXTURE_2D, 0);
}
