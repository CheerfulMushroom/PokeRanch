#include <iostream>
#include <opencv2/opencv.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"
#include "VideoStream.h"



VideoStream::VideoStream(int cam_index): cam(cv::VideoCapture(cam_index)) {
    glGenTextures(1, &texture);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);
    configure_VAO();

}


bool VideoStream::configure_VAO() {

    GLfloat vertices[] = {
            1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
            1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint indices[] = {
            0, 1, 3,
            1, 2, 3
    };

    shader = ShaderProgram("project/shaders/v_shader.txt", "project/shaders/f_shader.txt");

    glBindVertexArray(VAO);


    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);


    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader.get_program(), "ourTexture1"), 0);


    glBindVertexArray(0);
    glfwSwapInterval(1);

}

//TODO(me): add destructor
VideoStream::~VideoStream() {}


void VideoStream::render() {
    cam.read(frame);
    mat_to_texture();
    shader.use();

    glBindTexture(GL_TEXTURE_2D, texture);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);

    glBindTexture(GL_TEXTURE_2D, 0);
}


void VideoStream::mat_to_texture() {
    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    cv::flip(frame, frame, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());

    glBindTexture(GL_TEXTURE_2D, 0);
}
