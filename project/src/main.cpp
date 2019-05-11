#include <iostream>
#include <ShaderProgram.h>
#include <VideoStream.h>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include <opencv2/highgui.hpp>
#include </usr/local/include/aruco/aruco.h>
#include <GameWindow.h>

#define FPS 25

int main(int argc, char *argv[]) {
    

    const float marker_size = 0.04;


    cv::VideoCapture cam(1);

    GameWindow screen(800, 600, 25);


    VideoStream stream;

    stream.configure_VAO();    

    aruco::CameraParameters camera;
    camera.readFromXMLFile(argv[1]);

    aruco::MarkerDetector Detector;
    Detector.setDictionary("ARUCO_MIP_36h12");


    cv::Mat aruco_frame;

    while (!glfwWindowShouldClose(screen.window)) {
        screen.frame_start_time = glfwGetTime();

        glfwPollEvents();
        if (!cam.read(aruco_frame)) {
            break;
        }


        auto markers = Detector.detect(aruco_frame, camera, marker_size);

        for (auto m:markers) {
            aruco::CvDrawingUtils::draw3dAxis(aruco_frame, m, camera);
        }

        stream.frame = aruco_frame;

        stream.draw_video_frame();

        glfwSwapBuffers(screen.window);
        screen.lock_frame_rate();
    }

    glfwTerminate();
    return 0;
}
