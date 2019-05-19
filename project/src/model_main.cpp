#include <iostream>
#include <ShaderProgram.h>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include <opencv2/highgui.hpp>
#include <opencv2/calib3d.hpp>
#include <GameWindow.h>
#include <Model.h>
#include <Camera.h>
#include <VideoStream.h>
#include </usr/local/include/aruco/aruco.h>
#include "string.h"
#include "opencv2/core/types.hpp"
#include "math.h"

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

    double proj_mas[16];
        camera.glGetProjectionMatrix(camera.CamSize, screen.get_size(), proj_mas, 0.01, 100);
        glm::mat4 projection = glm::make_mat4(proj_mas);

    float rx = 6;
    float ry = 1;
    float rz = 1;
    
    while (!glfwWindowShouldClose(screen.window))
    {
        screen.frame_start_time = glfwGetTime();

        glClearColor(0.5f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Читаем кадр с камеры (захват и декодирование)
		inVid >> aruco_frame;

        stream.frame = aruco_frame;
        
        auto markers = MDetector.detect(aruco_frame, camera, marker_size);
        

        for (auto m:markers) {
            aruco::CvDrawingUtils::draw3dAxis(aruco_frame, m, camera);
       
        stream.render(); 
        glEnable(GL_DEPTH_TEST);
 

        shader.use();

        //glm::mat4 projection = glm::transpose(projection_T);
        //glm::mat4 projection = glm::transpose(projection_T);
        //glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) 800 / (float) 600, 0.1f, 100.0f); // Создается projection matrix
        
        glm::mat4 view = GL_camera.GetViewMatrix();  // Получает view matrix
        
        /*cv::Point3f cam_pose = camera.getCameraLocation(m.Rvec,m.Tvec);

        glm::mat4 view = glm::lookAt(
        glm::vec3(cam_pose.x,cam_pose.y,cam_pose.z), // Камера находится в мировых координатах (4,3,3)
        glm::vec3(m.get3DPoints().x,m.get3DPoints().y, m.get3DPoints().z), // И направлена в начало координат
        glm::vec3(0,1,0)  // "Голова" находится сверху
        );
        */

        shader.set_mat4_uniform("projection", projection); // Передали в шейдер матрицу проекции
        //shader.set_mat4_uniform("view", view);  // -//- матрицу вида

        cv::Mat trans = m.getTransformMatrix();

        /*for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) cout << trans.at<float>(i,j) << ' ';
            cout << endl;
                
        }
        */
         
        /*memcpy(glm::value_ptr(pikachu_mod), trans.data, 16 * sizeof(float));
        pikachu_mod = glm::transpose(pikachu_mod);
        glm::mat4 pikachu_mod_inv;
        glm::invert(pikachu_mod_inv, pikachu_mod);
        */
        
        cv::Mat rodrig;
        Rodrigues(m.Rvec, rodrig);
        glm::mat4 pikachu_mod = glm::mat4(1.0f);  // model matrix (translate, scale, rotate) 3v1
        pikachu_mod = glm::translate(pikachu_mod, glm::vec3(10 * m.Tvec.at<float>(0), 10 * m.Tvec.at<float>(1), -m.Tvec.at<float>(2) * 10)); // В ручную забили
        pikachu_mod = glm::scale(pikachu_mod, glm::vec3(0.02, 0.02, 0.02));
        pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-90.0f), glm::vec3(abs(m.Rvec.at<float>(0)), abs(m.Rvec.at<float>(1)), abs(m.Rvec.at<float>(2))));
        

        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians(90.0f), glm::vec3(1.0, 0.0, 0.0));
        rx -= 0.1;
        printf("\n%f %f %f\n", rx,ry,rz);
        if (rx <= -6) rx = 6;
        /*for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j) pikachu_mod[i][j] = trans.at<float>(j,i);
        */

        /*pikachu_mod[0][0] = 0.02;  // mat4[столбец][строка]
        pikachu_mod[1][0] = rodrig.at<float>(0,1);
        pikachu_mod[2][0] = rodrig.at<float>(0,2);
        pikachu_mod[3][0] = 100 * m.Tvec.at<float>(0);
        pikachu_mod[0][1] = rodrig.at<float>(1,0);
        pikachu_mod[1][1] = 0.02;
        pikachu_mod[2][1] = rodrig.at<float>(1,2);
        pikachu_mod[3][1] = 10 * m.Tvec.at<float>(1);
        pikachu_mod[0][2] = rodrig.at<float>(2,0);
        pikachu_mod[1][2] = rodrig.at<float>(2,1);
        pikachu_mod[2][2] = 0.02;
        pikachu_mod[3][2] = -100 * m.Tvec.at<float>(2);
        pikachu_mod[0][3] = 0;
        pikachu_mod[1][3] = 0;
        pikachu_mod[2][3] = 0;
        pikachu_mod[3][3] = 1;
        */

        //pikachu_mod = glm::translate(pikachu_mod, glm::vec3(m.Tvec.at<float>(0), m.Tvec.at<float>(1),m.Tvec.at<float>(2))); // В ручную забили
        //pikachu_mod = glm::scale(pikachu_mod, glm::vec3(0.02, 0.02, 0.02));
        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-90.0f), glm::vec3(m.Rvec.at<float>(0), m.Rvec.at<float>(1),m.Rvec.at<float>(2)));      
    

        printf("\n%f %f %f\n%f %f %f\n%f %f %f\n", m.Tvec.at<float>(0), m.Tvec.at<float>(1),m.Tvec.at<float>(2),0.01,0.01,0.01, m.Rvec.at<float>(0), m.Rvec.at<float>(1),m.Rvec.at<float>(2));
        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Вращение координат
        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Вращение координат

        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // Вращение координат
        //pikachu_mod = inverse(pikachu_mod);
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) cout << pikachu_mod[j][i] << ' ';
            cout << endl;
        }
        shader.set_mat4_uniform("model", pikachu_mod); // Передали матрицу модели
        
        pikachu_model.render(shader);

          // Или цикл должен закрываться ниже???

        glDisable(GL_DEPTH_TEST);

        }
        
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
