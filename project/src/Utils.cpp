#include "Utils.h"
#include "ShaderProgram.h"

// Перевод изображения в текстуру
void mat_to_texture(GLuint texture, cv::Mat pic, bool with_alpha) {
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

    cv::flip(pic, pic, 0);

    if (with_alpha) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, pic.cols, pic.rows, 0, GL_BGRA, GL_UNSIGNED_BYTE, pic.ptr());
    } else {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, pic.cols, pic.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, pic.ptr());
    }

    glBindTexture(GL_TEXTURE_2D, 0);
}

void configure_VAO(std::vector<GLfloat> vertices,
                   std::vector<GLuint> indices,
                   GLuint *VAO,
                   GLuint *VBO,
                   GLuint *EBO,
                   GLuint *texture,
                   ShaderProgram *shader) {
    glDeleteVertexArrays(1, VAO);
    glDeleteBuffers(1, VBO);
    glDeleteBuffers(1, EBO);


    glGenBuffers(1, VBO);
    glGenVertexArrays(1, VAO);
    glGenBuffers(1, EBO);

    glBindVertexArray(*VAO);


    glBindBuffer(GL_ARRAY_BUFFER, *VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (0 * sizeof(GLfloat)));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (void *) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    glGenTextures(1, texture);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, *texture);
    glUniform1i(glGetUniformLocation(shader->get_program(), "ourTexture1"), 0);


    glBindVertexArray(0);

}