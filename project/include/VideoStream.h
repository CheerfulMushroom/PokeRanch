#ifndef PROJECT_INCLUDE_VIDEOSTREAM_H_
#define PROJECT_INCLUDE_VIDEOSTREAM_H_

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <opencv2/opencv.hpp>
#include <unistd.h>
#include <opencv2/opencv.hpp>

#include "ShaderProgram.h"
#include "VideoStream.h"
class VideoStream {
 public:
    VideoStream();
    ~VideoStream();

    void render();
    
    bool configure_VAO();

    cv::Mat frame;


 private:
    void mat_to_texture();

    ShaderProgram shader;
    GLuint texture;
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
};

#endif // PROJECT_INCLUDE_VIDEOSTREAM_H_
