#include <opencv2/opencv.hpp>

#include <GL/glew.h>

#include "Picture.h"
#include "Utils.h"
#include "Utils.h"


Picture::Picture(GLfloat x, GLfloat y,
                 GLfloat x_size, GLfloat y_size,
                 std::string path_to_pic) : Renderable() {
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

