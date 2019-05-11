#ifndef PROJECT_INCLUDE_VIDEOSTREAM_H_
#define PROJECT_INCLUDE_VIDEOSTREAM_H_

#include <iostream>
#include <unistd.h>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ShaderProgram.h>
#include <opencv2/opencv.hpp>

class VideoStream {
 public:
    VideoStream();

    void draw_video_frame();

    bool configure_VAO();

    cv::Mat frame;

    GLuint texture;
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;


 private:
    void mat_to_texture();
    
};

#endif
