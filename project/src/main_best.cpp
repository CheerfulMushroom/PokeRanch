#include <iostream>
#include <ShaderProgram.h>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include <opencv2/highgui.hpp>
#include <GameWindow.h>
#include <AnimModel.h>
#include <Camera.h>
#include <VideoStream.h>
#include </usr/local/include/aruco/aruco.h>

#include <opencv2/calib3d.hpp>
#include <opencv2/core/types.hpp>


#include <sys/time.h>

#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>

#define marker_size 0.071

#define FPS 25

using namespace std;

int main(int argc, char *argv[]) {

    GameWindow screen(800, 600, 60);

    glEnable(GL_DEPTH_TEST);


    VideoStream stream;

    cv::VideoCapture cam(2);

    Camera GL_camera(glm::vec3(0.0f, 0.0f, 0.0f));

    stream.configure_VAO();

    aruco::MarkerDetector MDetector;
    MDetector.setDictionary("ARUCO_MIP_36h12");
    aruco::MarkerDetector::Params &params = MDetector.getParameters();
    aruco::DetectionMode dm = aruco::DM_FAST;
    float min_size = 0;
    MDetector.setDetectionMode(dm, min_size); // setDetectionMode(DetectionMode dm, float minMarkerSize=0)



    aruco::CameraParameters camera;
    camera.readFromXMLFile(argv[1]);  // Выбрать файл с характеристиками камеры (.yml) (второй аргумент командной строки)
	MDetector.setDictionary("ARUCO_MIP_36h12");


    cv::Size img_size(800, 600);


    double proj_mas[16];
    camera.glGetProjectionMatrix(camera.CamSize, img_size, proj_mas, 0.01, 100);
    glm::mat4 projection = glm::make_mat4(proj_mas);

    //GLfloat VIEW[16] = {0, 0, 0, 0,
                    //0, 0, 0, 0,
                    //0, 0, 0, 0,
                   // 0, 0, 0, 1};




    //glm::mat4 view = glm::make_mat4(VIEW);

    //std::cout << glm::to_string(view) << std::endl;

    glm::mat4 view = GL_camera.GetViewMatrix();

    
    AnimModel rockruff("models/Decidueye_walk.dae");

    cv::Mat aruco_frame;


    while (!glfwWindowShouldClose(screen.window))
    {
        screen.frame_start_time = glfwGetTime();

        glClearColor(0.5f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        cam.read(aruco_frame);


        auto markers = MDetector.detect(aruco_frame, camera, marker_size);

        stream.frame = aruco_frame;

        stream.render();



        aruco_frame = stream.frame;

        glEnable(GL_DEPTH_TEST);

        float RunningTime = glfwGetTime();

       
        for (auto m:markers) {
        cv::Mat rodrig;

        auto good_rvec = m.Rvec;
        good_rvec.at<float>(0, 2) *= -1.0;
        


        Rodrigues(good_rvec, rodrig);
        

        GLfloat RTMat[16] = {rodrig.at<float>(0, 0), rodrig.at<float>(0, 1), rodrig.at<float>(0, 2), 0,
                             rodrig.at<float>(1, 0), rodrig.at<float>(1, 1), rodrig.at<float>(1, 2), 0,
                             rodrig.at<float>(2, 0), rodrig.at<float>(2, 1), rodrig.at<float>(2, 2), 0,
                             10 * m.Tvec.at<float>(0), 10 * m.Tvec.at<float>(1), 10 * -m.Tvec.at<float>(2), 1};


        glm::mat4 pikachu_mod = glm::make_mat4(RTMat);

        pikachu_mod = glm::scale(pikachu_mod, glm::vec3(0.02, 0.02, 0.02));
        
        pikachu_mod = glm::rotate(pikachu_mod, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        
        rockruff.update(RunningTime, projection, view, pikachu_mod); 
        rockruff.render();

        }

        glDisable(GL_DEPTH_TEST);

        glfwSwapBuffers(screen.window);
        glfwPollEvents();

        screen.lock_frame_rate();
    }

    glfwTerminate();
    return 0;
}
