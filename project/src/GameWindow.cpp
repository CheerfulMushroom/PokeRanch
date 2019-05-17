#include <iostream>
#include <GameWindow.h>
#include <unistd.h>

GameWindow::GameWindow(int width, int height, double rate) : window_width(width), window_height(height), fps(rate) {
    screen_init();
}

bool GameWindow::screen_init() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

    window = glfwCreateWindow(window_width, window_height, "Hello", nullptr, nullptr);

    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return false;
    }

    glfwMakeContextCurrent(window);
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to init GLEW" << std::endl;
        glfwTerminate();
        return false;
    }


    glViewport(0, 0, window_width, window_height);
    return true;
}


void GameWindow::lock_frame_rate() {
    double allowed_frame_time = 1.0 / fps;

    frame_end_time = glfwGetTime();
    double frame_draw_time = frame_end_time - frame_start_time;
    
    double sleep_time = 0.0;

    if (frame_draw_time < allowed_frame_time) {
        sleep_time = allowed_frame_time - frame_draw_time;
        usleep(1000000 * sleep_time);
    }
}

cv::Size GameWindow::get_size() {
	cv::Size img_size(window_width, window_height);
	return img_size;
}
