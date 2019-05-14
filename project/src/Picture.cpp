#include <opencv2/opencv.hpp>

#include "Picture.h"
#include "Utils.h"

Picture::Picture(GLfloat x, GLfloat y,
                 GLfloat x_size, GLfloat y_size,
                 std::string path_to_pic,
                 bool has_alpha) : Renderable() {
    shader = ShaderProgram("project/shaders/v_shader.txt", "project/shaders/f_shader.txt");


    GLfloat vertices[] = {
            x + x_size, y + y_size, 0.0f, 1.0f, 1.0f,
            x + x_size, y, 0.0f, 1.0f, 0.0f,
            x, y, 0.0f, 0.0f, 0.0f,
            x, y + y_size, 0.0f, 0.0f, 1.0f
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

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader.get_program(), "ourTexture1"), 0);


    glBindVertexArray(0);

    if (has_alpha) {
        auto icon = cv::imread(path_to_pic, -1);
        mat_to_texture(texture, icon, true);
    } else{
        auto icon = cv::imread(path_to_pic);
        mat_to_texture(texture, icon, false);
    }

}


Picture::~Picture() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
}


void Picture::render() {
    shader.use();
    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, nullptr);

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}

