#ifndef PROJECT_INCLUDE_GAMEWINDOW_H_
#define PROJECT_INCLUDE_GAMEWINDOW_H_

#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class GameWindow {
 public:
    GameWindow() = default;

    GameWindow(int width, int height);

    bool screen_init();


    GLFWwindow *window;

 private:
    int window_width;
    int window_height;
};

#endif
