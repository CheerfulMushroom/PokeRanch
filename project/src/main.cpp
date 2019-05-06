#include <iostream>
#include <ShaderProgram.h>
#include <VideoStream.h>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>

#define FPS 25

int main(void) {
    

    cv::VideoCapture cam(1);

    VideoStream stream(800, 600);

    stream.configure_VAO();    

    while (!glfwWindowShouldClose(stream.window)) {
        stream.frame_start_time = glfwGetTime();

        glfwPollEvents();
        if (!cam.read(stream.frame)) {
            break;
        }

        stream.draw_video_frame();

        glfwSwapBuffers(stream.window);
        stream.lock_frame_rate(FPS);
    }

    glfwTerminate();
    return 0;
}









