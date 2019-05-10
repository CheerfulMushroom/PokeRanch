#ifndef PROJECT_INCLUDE_VIDEOSTREAM_H_
#define PROJECT_INCLUDE_VIDEOSTREAM_H_

#include <iostream>
#include <unistd.h>
#include <opencv2/opencv.hpp>


#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "ShaderProgram.h"
#include "VideoStream.h"
#include "Interfaces.h"


class VideoStream: public Renderable {
 public:
    VideoStream();

    void render();

    //bool context_init();

    bool configure_VAO();

    //void lock_frame_rate(double frame_rate);

    //GLFWwindow *window;

    //double frame_start_time;
    //double frame_end_time;

    cv::Mat frame;

    GLuint texture;
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;


 private:
    void mat_to_texture();

    //int window_width;
    //int window_height;

};

#endif
