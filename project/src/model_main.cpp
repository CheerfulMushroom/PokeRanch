#include <iostream>
#include <ShaderProgram.h>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include <opencv2/highgui.hpp>
#include <GameWindow.h>
#include <Model_anim.h>
#include <Camera.h>
#include <VideoStream.h>
#include </usr/local/include/aruco/aruco.h>

#define marker_size 0.07

#define FPS 25

int main(int argc, char *argv[]) {

    GameWindow screen(800, 600, 25);

    glEnable(GL_DEPTH_TEST);

    Camera GL_camera(glm::vec3(0.0f, 0.0f, 5.0f));  // Создает камеру

    VideoStream stream;

    cv::VideoCapture cam(0);

    stream.configure_VAO();

    aruco::CameraParameters camera;
    camera.readFromXMLFile(argv[1]);
    aruco::MarkerDetector Detector;
    Detector.setDictionary("ARUCO_MIP_36h12");
    
    ShaderProgram shader("project/src/v_model_shader.txt", "project/src/f_model_shader.txt");

    Model pikachu_model("models/pikachu.dae");

    cv::Mat aruco_frame;

    while (!glfwWindowShouldClose(screen.window))
    {
        screen.frame_start_time = glfwGetTime();

        glClearColor(0.5f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        if (!cam.read(aruco_frame)) {
            break;
        }

          
        auto markers = Detector.detect(aruco_frame, camera, marker_size);
  
        for (auto m:markers) {
            aruco::CvDrawingUtils::draw3dAxis(aruco_frame, m, camera);
        }

        stream.frame = aruco_frame;

        stream.render();

        glEnable(GL_DEPTH_TEST);

        shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) 800 / (float) 600, 0.1f, 100.0f); // Создается projection matrix
        
        glm::mat4 view = GL_camera.GetViewMatrix();  // Получает view matrix

        shader.set_mat4_uniform("projection", projection); // Передали в шейдер матрицу проекции
        shader.set_mat4_uniform("view", view);  // -//- матрицу вида

        glm::mat4 pikachu_mod = glm::mat4(1.0f);  // model matrix (translate, scale, rotate) 3v1
        pikachu_mod = glm::translate(pikachu_mod, glm::vec3(0.5f, -0.4f, 0.0f)); // В ручную забили
        pikachu_mod = glm::scale(pikachu_mod, glm::vec3(0.02, 0.02, 0.02));
        pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));      

        shader.set_mat4_uniform("model", pikachu_mod); // Передали матрицу модели

        pikachu_model.render(shader);

        glDisable(GL_DEPTH_TEST);

        glfwSwapBuffers(screen.window);
        glfwPollEvents();

        screen.lock_frame_rate();
    }

    glfwTerminate();
    return 0;
}

/* Что нужно получить с ArUco ? 

    Проекционная матрица (составляется из параметров камеры) 4x4

    Матрица modelview (view * model) составляет из rvec, tvec
*/
