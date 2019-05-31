#include <iostream>
#include <ShaderProgram.h>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <GameWindow.h>
#include <AnimModel.h>
//#include <Model.h>
#include <Camera.h>
#include <VideoStream.h>
#include </usr/local/include/aruco/aruco.h>
#include <stdio.h>
#include <stdlib.h>
#include "string.h"
#include "opencv2/core/types.hpp"
#include "math.h"
#include "pokemon_model_path.h"

#include <sys/time.h>

#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#define marker_size 0.073

#define FPS 60

using namespace std;

int main(int argc, char *argv[]) {

	if (argc != 3) {
        cout << "[camera number] [camera_calibration_file.yml]" << endl;
        return 0;
    }

    GameWindow screen(800, 600, FPS);

    glEnable(GL_DEPTH_TEST);

    VideoStream stream;

    cv::VideoCapture inVid(stoi(argv[1])); // Открыть камеру по умолчанию (первый аргумент командной сроки)
        if (!inVid.isOpened()) { // Проверка ошибок
            cout << "Ошибка! Камера не готова...\n";
            return -1;
    }

    
    Camera GL_camera(glm::vec3(0.0f, 0.0f, 0.0f));

    
    stream.configure_VAO();

    aruco::MarkerDetector MDetector;
    MDetector.setDictionary("ARUCO_MIP_36h12");
    aruco::MarkerDetector::Params &params = MDetector.getParameters();
    aruco::DetectionMode dm = aruco::DM_FAST;
    float min_size = 0;
    MDetector.setDetectionMode(dm, min_size); // setDetectionMode(DetectionMode dm, float minMarkerSize=0)
    
    //ShaderProgram shader("project/src/v_model_shader.txt", "project/src/f_model_shader.txt");

    //Model pikachu_model("models/pm0844_00_fi.dae");

	aruco::CameraParameters camera;
    camera.readFromXMLFile(argv[2]);  // Выбрать файл с характеристиками камеры (.yml) (второй аргумент командной строки)
	MDetector.setDictionary("ARUCO_MIP_36h12");

    cv::Size img_size(800, 600);
    
    double proj_mas[16];
    camera.glGetProjectionMatrix(camera.CamSize, img_size, proj_mas, 0.01, 100);
    glm::mat4 projection = glm::make_mat4(proj_mas);
 
    glm::mat4 view = GL_camera.GetViewMatrix();
    
    /*char* path = pokemon_model_path(3, "walk");
    
    AnimModel rockruff(path);
    delete(path);
    */

    cv::Mat aruco_frame;

    float coord = -2;

    
    while (!glfwWindowShouldClose(screen.window))
    {
        screen.frame_start_time = glfwGetTime();

        glClearColor(0.5f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Читаем кадр с камеры (захват и декодирование)
		inVid >> aruco_frame;

        
        auto markers = MDetector.detect(aruco_frame, camera, marker_size);
        
        stream.frame = aruco_frame;

        stream.render();
        
        aruco_frame = stream.frame;

        glEnable(GL_DEPTH_TEST);

       
        float RunningTime = glfwGetTime();
        
        
        for (auto m:markers) aruco::CvDrawingUtils::draw3dAxis(aruco_frame, m, camera);

        for (auto m:markers) {
       
        char* path = pokemon_model_path(m.id, "stay");
        AnimModel current_model(path);
        delete(path);

        
        //glEnable(GL_DEPTH_TEST);
        cv::Mat rodrig;

        auto good_rvec = m.Rvec;
        good_rvec.at<float>(0, 2) *= -1.0;

        Rodrigues(good_rvec, rodrig);

        //shader.use();

         GLfloat RTMat[16] = {rodrig.at<float>(0, 0), rodrig.at<float>(0, 1), rodrig.at<float>(0, 2), 0,
                             rodrig.at<float>(1, 0), rodrig.at<float>(1, 1), rodrig.at<float>(1, 2), 0,
                             rodrig.at<float>(2, 0), rodrig.at<float>(2, 1), rodrig.at<float>(2, 2), 0,
                             10 * m.Tvec.at<float>(0), 10 * m.Tvec.at<float>(1) + coord, 10 * -m.Tvec.at<float>(2), 1};

        coord += 0.1;
        
        glm::mat4 pikachu_mod = glm::make_mat4(RTMat);


        pikachu_mod = glm::scale(pikachu_mod, glm::vec3(0.06, 0.06, 0.06));

        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        //cv::Mat viewMatrix = cv::Mat::zeros(4, 4, CV_32F);
        
        /*viewMatrix.at<float>(0, 3) = 0;
	    viewMatrix.at<float>(1, 3) = 0;
	    viewMatrix.at<float>(2, 3) = 0;
        viewMatrix.at<float>(3, 3) = 1;
        */

        current_model.update(RunningTime, projection, view, pikachu_mod);
        current_model.render();
        
        
        }
        
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
