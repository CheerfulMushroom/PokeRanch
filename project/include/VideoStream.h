#ifndef PROJECT_INCLUDE_VIDEOSTREAM_H_
#define PROJECT_INCLUDE_VIDEOSTREAM_H_

#include <iostream>
#include <unistd.h>
#include <opencv2/opencv.hpp>


#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "ShaderProgram.h"
#include "VideoStream.h"
#include "Interfaces.h"


class VideoStream: public Renderable {
 public:
    VideoStream(int cam_index);
    ~VideoStream();

    void render();
    
    bool configure_VAO();


 private:
    void mat_to_texture();

    ShaderProgram shader;
    cv::VideoCapture cam;
    cv::Mat frame;
    GLuint texture;
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
};

#endif // PROJECT_INCLUDE_VIDEOSTREAM_H_
