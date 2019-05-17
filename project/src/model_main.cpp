#include <iostream>
#include <ShaderProgram.h>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include <opencv2/highgui.hpp>
#include <GameWindow.h>
#include <Model.h>
#include <Camera.h>
#include <VideoStream.h>
#include </usr/local/include/aruco/aruco.h>
#include "string.h"

#define marker_size 0.071

#define FPS 25

using namespace std;

int main(int argc, char *argv[]) {

	if (argc != 3) {
        cout << "[camera number] [camera_calibration_file.yml]" << endl;
        return 0;
    }

    GameWindow screen(800, 600, 25);

    glEnable(GL_DEPTH_TEST);

    Camera GL_camera(glm::vec3(0.0f, 0.0f, 5.0f));  // Создает камеру

    VideoStream stream;

    cv::VideoCapture inVid(stoi(argv[1])); // Открыть камеру по умолчанию (первый аргумент командной сроки)
    	if (!inVid.isOpened()) { // Проверка ошибок
        	cout << "Ошибка! Камера не готова...\n";
        	return -1;
    }

    stream.configure_VAO();

    aruco::MarkerDetector MDetector;
    MDetector.setDictionary("ARUCO_MIP_36h12");
    aruco::MarkerDetector::Params &params = MDetector.getParameters();
    aruco::DetectionMode dm = aruco::DM_FAST;
    float min_size = 0;
    MDetector.setDetectionMode(dm, min_size); // setDetectionMode(DetectionMode dm, float minMarkerSize=0)
    
    ShaderProgram shader("project/src/v_model_shader.txt", "project/src/f_model_shader.txt");

    Model pikachu_model("models/pikachu.dae");

    cv::Mat aruco_frame;	
	aruco::CameraParameters camera;
    camera.readFromXMLFile(argv[2]);  // Выбрать файл с характеристиками камеры (.yml) (второй аргумент командной строки)
	MDetector.setDictionary("ARUCO_MIP_36h12");

    while (!glfwWindowShouldClose(screen.window))
    {
        screen.frame_start_time = glfwGetTime();

        glClearColor(0.5f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Читаем кадр с камеры (захват и декодирование)
		inVid >> aruco_frame;

          
        auto markers = MDetector.detect(aruco_frame, camera, marker_size);
  
        for (auto m:markers) {
            //aruco::CvDrawingUtils::draw3dAxis(aruco_frame, m, camera);
        
        stream.frame = aruco_frame;

        stream.render();

        glEnable(GL_DEPTH_TEST);

        double proj_mas[16];
        camera.glGetProjectionMatrix(camera.CamSize, screen.get_size(), proj_mas, 0.1, 100, false);
        
        shader.use();

        glm::mat4 projection = glm::make_mat4(proj_mas);
        //glm::mat4 projection = glm::transpose(projection_T);
        //glm::mat4 projection = glm::transpose(projection_T);
        //glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) 800 / (float) 600, 0.1f, 100.0f); // Создается projection matrix
        
        //glm::mat4 view = GL_camera.GetViewMatrix();  // Получает view matrix


        shader.set_mat4_uniform("projection", projection); // Передали в шейдер матрицу проекции
        //shader.set_mat4_uniform("view", view);  // -//- матрицу вида

        glm::mat4 pikachu_mod = glm::mat4(1.0f);  // model matrix (translate, scale, rotate) 3v1
        pikachu_mod = glm::translate(pikachu_mod, glm::vec3(m.Tvec.at<float>(0), m.Tvec.at<float>(1),m.Rvec.at<float>(2))); // В ручную забили
        pikachu_mod = glm::scale(pikachu_mod, glm::vec3(0.01, 0.01, 0.01));
        pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-90.0f), glm::vec3(m.Rvec.at<float>(0), m.Rvec.at<float>(1),m.Rvec.at<float>(2)));      
        
        shader.set_mat4_uniform("modelview", pikachu_mod); // Передали матрицу модели
        
        pikachu_model.render(shader);

        }  // Или цикл должен закрываться ниже???

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

    Матрица modelview (view * model) составляется из rvec, tvec
*/
