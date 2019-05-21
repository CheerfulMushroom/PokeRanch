#include <opencv2/opencv.hpp>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "MovingPicture.h"
#include "Utils.h"

MovingPicture::MovingPicture(GLfloat x, GLfloat y,
                             GLfloat x_size, GLfloat y_size,
                             std::function<GLfloat(GLfloat)> x_movement,
                             std::function<GLfloat(GLfloat)> y_movement,
                             std::string path_to_pic) {
    this->x = x;
    this->y = y;
    this->x_size = x_size;
    this->y_size = y_size;
    this->x_movement = std::move(x_movement);
    this->y_movement = std::move(y_movement);
    shader = ShaderProgram("project/shaders/v_shader.txt", "project/shaders/f_shader.txt");


    std::vector<GLfloat> vertices = {
            x + x_size, y + y_size, 0.0f, 1.0f, 1.0f,
            x + x_size, y, 0.0f, 1.0f, 0.0f,
            x, y, 0.0f, 0.0f, 0.0f,
            x, y + y_size, 0.0f, 0.0f, 1.0f
    };

    std::vector<GLuint> indices = {
            0, 1, 3,
            1, 2, 3
    };


    configure_VAO(vertices, indices, &VAO, &VBO, &EBO, &texture, &shader);

    auto pic = cv::imread(path_to_pic, -1);
    bool has_alpha = pic.channels() == 4;
    mat_to_texture(texture, pic, has_alpha);
}


MovingPicture::~MovingPicture() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void MovingPicture::render() {
    shader.use();
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void MovingPicture::update() {
    x = (GLfloat) x_movement(glfwGetTime());
    y = (GLfloat) y_movement(glfwGetTime());

    std::vector<GLfloat> vertices = {
            x + x_size, y + y_size, 0.0f, 1.0f, 1.0f,
            x + x_size, y, 0.0f, 1.0f, 0.0f,
            x, y, 0.0f, 0.0f, 0.0f,
            x, y + y_size, 0.0f, 0.0f, 1.0f
    };

    std::vector<GLuint> indices = {
            0, 1, 3,
            1, 2, 3
    };


    configure_VAO(vertices, indices, &VAO, &VBO, &EBO, &texture, &shader);
}
