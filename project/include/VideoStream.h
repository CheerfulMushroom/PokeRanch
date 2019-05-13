#ifndef PROJECT_INCLUDE_VIDEOSTREAM_H_
#define PROJECT_INCLUDE_VIDEOSTREAM_H_

#include <iostream>
#include <unistd.h>
#include <opencv2/opencv.hpp>


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"
#include "Interfaces.h"

class VideoStream: public Renderable {
 public:
    explicit VideoStream(int cam_index);
    VideoStream() = default;
    ~VideoStream();

    void render() override;

    //bool context_init();

    bool configure_VAO();

    //void lock_frame_rate(double frame_rate);

    //GLFWwindow *window;

    //double frame_start_time;
    //double frame_end_time;




 private:
    ShaderProgram shader;
    cv::VideoCapture cam;
    cv::Mat frame;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture;
    //int window_width;
    //int window_height;

};

#endif
