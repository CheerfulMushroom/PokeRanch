#include <iostream>
#include <ShaderProgram.h>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include <opencv2/highgui.hpp>
#include <GameWindow.h>
#include <ogldev_skinned_mesh.h>
#include <Camera.h>
#include <VideoStream.h>
#include </usr/local/include/aruco/aruco.h>

#include <sys/time.h>

#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#define marker_size 0.07

#define FPS 25

using namespace std;

int main(int argc, char *argv[]) {

    GameWindow screen(800, 600, 60);

    glEnable(GL_DEPTH_TEST);

    Camera GL_camera(glm::vec3(0.5f, 0.0f, 4.0f));

    VideoStream stream;

    cv::VideoCapture cam(1);

    stream.configure_VAO();
    
    SkinnedMesh rockruff("models/rockruff.dae");

    cv::Mat aruco_frame;


    while (!glfwWindowShouldClose(screen.window))
    {
        screen.frame_start_time = glfwGetTime();

        glClearColor(0.5f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (!cam.read(aruco_frame)) {
            break;
        }
  
        stream.frame = aruco_frame;

        stream.render();

        glEnable(GL_DEPTH_TEST);

        float RunningTime = glfwGetTime();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) 800 / (float) 600, 0.1f, 100.0f);
        
        glm::mat4 view = GL_camera.GetViewMatrix();

        glm::mat4 pikachu_mod = glm::mat4(1.0f);
        pikachu_mod = glm::translate(pikachu_mod, glm::vec3(-1.0f, -0.4f, 0.0f));
        pikachu_mod = glm::scale(pikachu_mod, glm::vec3(0.02, 0.02, 0.02));
        pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));      
        pikachu_mod = glm::rotate(pikachu_mod, glm::radians(45.0f), glm::vec3(0.0f, 0.0, 1.0f));

        rockruff.update(RunningTime, projection, view, pikachu_mod);

        rockruff.Render();

        glDisable(GL_DEPTH_TEST);

        glfwSwapBuffers(screen.window);
        glfwPollEvents();

        screen.lock_frame_rate();
    }

    glfwTerminate();
    return 0;
}
