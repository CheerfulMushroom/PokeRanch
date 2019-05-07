#ifndef PROJECT_INCLUDE_GAMEWINDOW_H_
#define PROJECT_INCLUDE_GAMEWINDOW_H_



#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <ShaderProgram.h>
#include <opencv2/opencv.hpp>

class GameWindow {
 public:
    GameWindow(int width, int height, double rate);

    bool screen_init();

    void lock_frame_rate();
    

    GLFWwindow *window;

    double frame_start_time;
    double frame_end_time;

 private:
    double fps;
    int window_width;
    int window_height;
};

#endif
