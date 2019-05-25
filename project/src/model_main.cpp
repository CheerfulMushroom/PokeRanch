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

cv::Mat eulerAnglesToRotationMatrix(cv::Vec3f &theta) {
    // Calculate rotation about x axis
    cv::Mat R_x = (cv::Mat_<float>(3,3) <<
               1,       0,              0,
               0,       cos(theta[0]),   -sin(theta[0]),
               0,       sin(theta[0]),   cos(theta[0])
               );

    // Calculate rotation about y axis
    cv::Mat R_y = (cv::Mat_<float>(3,3) <<
               cos(theta[2]),    0,      sin(theta[2]),
               0,               1,      0,
               -sin(theta[2]),   0,      cos(theta[2])
               );

    // Calculate rotation about z axis
    cv::Mat R_z = (cv::Mat_<float>(3,3) <<
               cos(theta[1]),    -sin(theta[1]),      0,
               sin(theta[1]),    cos(theta[1]),       0,
               0,               0,                  1);


    // Combined rotation matrix
    cv::Mat R = R_x * R_y * R_z;

    return R;

}


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

    Model pikachu_model("models/pm0844_00_fi.dae");

    cv::Mat aruco_frame;	
	aruco::CameraParameters camera;
    camera.readFromXMLFile(argv[2]);  // Выбрать файл с характеристиками камеры (.yml) (второй аргумент командной строки)
	MDetector.setDictionary("ARUCO_MIP_36h12");

    double proj_mas[16];
        camera.glGetProjectionMatrix(camera.CamSize, screen.get_size(), proj_mas, 0.01, 100); // Транспонировать?
        glm::mat4 projection = glm::make_mat4(proj_mas);
    
        //projection = transpose(projection);

    float rx;
    float ry;
    float rz;
    float pi = 3.1415926535;
    float grad = 1;
    cv::Mat transf;
    while (!glfwWindowShouldClose(screen.window))
    {
        screen.frame_start_time = glfwGetTime();

        glClearColor(0.5f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


        // Читаем кадр с камеры (захват и декодирование)
		inVid >> aruco_frame;

        stream.frame = aruco_frame;
        
        auto markers = MDetector.detect(aruco_frame, camera, marker_size);
        
        stream.render();
        for (auto m:markers) aruco::CvDrawingUtils::draw3dAxis(aruco_frame, m, camera);

        for (auto m:markers) {
            //aruco::CvDrawingUtils::draw3dAxis(aruco_frame, m, camera);
       
        glEnable(GL_DEPTH_TEST);
 

        shader.use();

        //glm::mat4 projection = glm::transpose(projection_T);
        //glm::mat4 projection = glm::transpose(projection_T);
        //glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) 800 / (float) 600, 0.1f, 100.0f); // Создается projection matrix
        //cv::Point3f camera_loc = camera.getCameraLocation(m.Rvec, m.Tvec); 
        //glm::mat4 view = glm::lookAt(glm::vec3(camera_loc.x,camera_loc.z,camera_loc.y),glm::vec3(0.,0.,0.),glm::vec3(0.,0.,1.));
        glm::mat4 view_test = GL_camera.GetViewMatrix();  // Получает view matrix
        shader.set_mat4_uniform("view", view_test); 


        
        /*cv::Point3f cam_pose = camera.getCameraLocation(m.Rvec,m.Tvec);

        glm::mat4 view = glm::lookAt(
        glm::vec3(cam_pose.x,cam_pose.y,cam_pose.z), // Камера находится в мировых координатах (4,3,3)
        glm::vec3(m.get3DPoints().x,m.get3DPoints().y, m.get3DPoints().z), // И направлена в начало координат
        glm::vec3(0,1,0)  // "Голова" находится сверху
        );
        */

        cv::Mat viewMatrix = cv::Mat::zeros(4, 4, CV_32F);
        
        viewMatrix.at<float>(0, 3) = 0;
	    viewMatrix.at<float>(1, 3) = 0;
	    viewMatrix.at<float>(2, 3) = 0;
        viewMatrix.at<float>(3, 3) = 1;
        



        cv::Mat rot_mat;
        cv::Rodrigues(m.Rvec, rot_mat);

        for (unsigned int row = 0; row < 3; ++row) {
			
            for (unsigned int col = 0; col < 3; ++col) {
				viewMatrix.at<float>(row, col) = (float)rot_mat.at<float>(row, col);
			}
		
        }
        
        viewMatrix.at<float>(0, 3) = (float)m.Tvec.at<float>(0) * 0.1f;
        viewMatrix.at<float>(1, 3) = -(float)m.Tvec.at<float>(2) * 0.1f;
        viewMatrix.at<float>(2, 3) = 0.1f * (float)m.Tvec.at<float>(1);

        viewMatrix.at<float>(3, 3) = 1.0f;

        cv::Mat cvToGl = cv::Mat::zeros(4, 4, CV_32F);
		
        cvToGl.at<float>(0, 0) = 1.0f;
		cvToGl.at<float>(1, 1) = -1.0f; // Invert the y axis
		cvToGl.at<float>(2, 2) = -1.0f; // invert the z axis
		cvToGl.at<float>(3, 3) = 1.0f;
		
        viewMatrix = cvToGl * viewMatrix;
        
        cv::transpose(viewMatrix, viewMatrix); 

        glm::mat4 view;        
        
        for (unsigned int i = 0; i < 4; ++i)
            for (unsigned int j = 0; j < 4; ++j) {
                view[i][j] = viewMatrix.at<float>(i,j);
            }

        //view = inverse(view);
        
        shader.set_mat4_uniform("projection", projection); // Передали в шейдер матрицу проекции
        
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) cout << view[j][i] << ' ';
            cout << endl;
        }

        
        shader.set_mat4_uniform("model", view);  // -//- матрицу вида

        //cv::Mat trans = m.getTransformMatrix();

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
        //rx = m.Rvec.at<float>(0);
        //ry = -m.Rvec.at<float>(2);
        //rz = m.Rvec.at<float>(1);
        //cv::Vec3f rvec = m.Rvec;
        //cv::Mat rodrig = eulerAnglesToRotationMatrix(rvec);
        //Rodrigues(m.Rvec, rodrig);
        glm::mat4 pikachu_mod = glm::mat4(1.0f);  // model matrix (translate, scale, rotate) 3v1
        //pikachu_mod = glm::translate(pikachu_mod, glm::vec3(m.Tvec.at<float>(0), -m.Tvec.at<float>(2), m.Tvec.at<float>(1))); // В ручную забили
        pikachu_mod = glm::scale(pikachu_mod, glm::vec3(0.02, 0.02, 0.02));
        
        //aruco::MarkerPoseTracker mpt;
        //mpt.estimatePose(m, camera, marker_size);
        //transf = mpt.getRTMatrix();
        
        //for (int i = 0; i < 3; ++i)
            //for (int j = 0; j < 3; ++j) pikachu_mod[j][i] = rodrig.at<float>(i,j);

        pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-90.0f), glm::vec3(1.0, 0.0, 0.0));
        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-90.0f), glm::vec3(rx<0?pi-abs(rx):rx, ry<0?pi-abs(ry):ry, rz<0?pi-abs(rz):rz));


        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians(250.0f), glm::vec3(1.0, 0.0, 0.0));
        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-5.0f), glm::vec3(0.0, 1.0, 0.0));
        
        
        /*pikachu_mod = glm::rotate(pikachu_mod, m.Rvec.at<float>(0), glm::vec3(1.0, 0.0, 0.0));
        pikachu_mod = glm::rotate(pikachu_mod, m.Rvec.at<float>(1), glm::vec3(0.0, 1.0, 0.0));
        pikachu_mod = glm::rotate(pikachu_mod, -m.Rvec.at<float>(2), glm::vec3(0.0, 0.0, 1.0));
        */

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

        //pikachu_mod = glm::translate(pikachu_mod, glm::vec3(m.Tvec.at<float>(0), m.Tvec.at<float>(1),-m.Tvec.at<float>(2))); // В ручную забили
        //pikachu_mod = glm::scale(pikachu_mod, glm::vec3(0.02, 0.02, 0.02));
        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-90.0f), glm::vec3(m.Rvec.at<float>(0), m.Rvec.at<float>(1),m.Rvec.at<float>(2)));      
    

        printf("\n%f %f %f\n%f %f %f\n%f %f %f\n", m.Tvec.at<float>(0), m.Tvec.at<float>(1),m.Tvec.at<float>(2),0.01,0.01,0.01, m.Rvec.at<float>(0), m.Rvec.at<float>(1),m.Rvec.at<float>(2));
        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Вращение координат
        /*pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-140.0f), glm::vec3(0.0f, 0.0f, 1.0f)); // Вращение координат
        pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-30.0f), glm::vec3(1.0f, 0.0f, 0.0f)); // Вращение координат
        */

        //pikachu_mod = glm::rotate(pikachu_mod, glm::radians((grad++)), glm::vec3(0.0f, 1.0f, 0.0f)); // Вращение координат
        //pikachu_mod = inverse(pikachu_mod);
        /*for (int i = 0; i < 4; ++i) {
            viewMatrix = for (int j = 0; j < 4; ++j) cout << pikachu_mod[j][i] << ' ';
            cout << endl;
        }
        */

        //shader.set_mat4_uniform("model", pikachu_mod); // Передали матрицу модели
        //printf("\nGrad: %f Rad: %f\n", grad, glm::radians(grad++));
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
