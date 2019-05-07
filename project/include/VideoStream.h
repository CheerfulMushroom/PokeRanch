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
