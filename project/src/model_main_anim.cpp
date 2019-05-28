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

long long m_startTime = 0;

int main(int argc, char *argv[]) {

    GameWindow screen(1280, 720, 25);

    glEnable(GL_DEPTH_TEST);

    Camera GL_camera(glm::vec3(0.0f, 0.0f, 1.0f));  // Создает камеру

    VideoStream stream;

    cv::VideoCapture cam(1);

    stream.configure_VAO();

    aruco::CameraParameters camera;
    camera.readFromXMLFile(argv[1]);
    aruco::MarkerDetector Detector;
    Detector.setDictionary("ARUCO_MIP_36h12");
    
    ShaderProgram shader("project/src/v_model_anim_shader.txt", "project/src/f_model_anim_shader.txt");

    SkinnedMesh slowpoke("models/slowpoke.dae");
    SkinnedMesh litten("models/litten.dae");
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
  
        auto markers = Detector.detect(aruco_frame, camera, marker_size);
  
        for (auto m:markers) {
            aruco::CvDrawingUtils::draw3dAxis(aruco_frame, m, camera);
        }

        stream.frame = aruco_frame;

        stream.render();

        glEnable(GL_DEPTH_TEST);

        shader.use();

        float RunningTime = glfwGetTime();
         
        vector<Matrix4f> Transforms;
        rockruff.BoneTransform(RunningTime, Transforms);

        for (unsigned int i = 0; i < Transforms.size(); ++i)
        {
            const std::string name = "gBones[" + std::to_string(i) + "]";
            GLuint boneTransform = glGetUniformLocation(shader.program, name.c_str());
            glUniformMatrix4fv(boneTransform, 1, GL_TRUE, (const GLfloat*) Transforms[i]);
        }

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) 800 / (float) 600, 0.1f, 100.0f); // Создается projection matrix
        
        glm::mat4 view = GL_camera.GetViewMatrix();  // Получает view matrix

        shader.set_mat4_uniform("projection", projection); // Передали в шейдер матрицу проекции
        shader.set_mat4_uniform("view", view);  // -//- матрицу вида

        glm::mat4 pikachu_mod = glm::mat4(1.0f);  // model matrix (translate, scale, rotate) 3v1
        pikachu_mod = glm::translate(pikachu_mod, glm::vec3(-1.0f, -0.4f, 0.0f)); // В ручную забили
        pikachu_mod = glm::scale(pikachu_mod, glm::vec3(0.02, 0.02, 0.02));
        pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));      
        pikachu_mod = glm::rotate(pikachu_mod, glm::radians(45.0f), glm::vec3(0.0f, 0.0, 1.0f));
        shader.set_mat4_uniform("model", pikachu_mod); // Передали матрицу модели

        rockruff.Render(shader);




        vector<Matrix4f> Transforms1;
        litten.BoneTransform(RunningTime, Transforms1);
  
        for (unsigned int i = 0; i < Transforms1.size(); ++i)
        {
              const std::string name = "gBones[" + std::to_string(i) + "]";
              GLuint boneTransform = glGetUniformLocation(shader.program, name.c_str());
              glUniformMatrix4fv(boneTransform, 1, GL_TRUE, (const GLfloat*) Transforms1[i]);
         }

        glm::mat4 pikachu_mod1 = glm::mat4(1.0f);
        pikachu_mod1 = glm::translate(pikachu_mod1, glm::vec3(0.2f, -0.4f, 0.0f)); // В ручную забили
        pikachu_mod1 = glm::scale(pikachu_mod1, glm::vec3(0.02, 0.02, 0.02));
        pikachu_mod1 = glm::rotate(pikachu_mod1, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));

        shader.set_mat4_uniform("model", pikachu_mod1);
        litten.Render(shader);


        vector<Matrix4f> Transforms2;
        slowpoke.BoneTransform(RunningTime, Transforms2);
 
        for (unsigned int i = 0; i < Transforms2.size(); ++i)
        {
            const std::string name = "gBones[" + std::to_string(i) + "]";
            GLuint boneTransform = glGetUniformLocation(shader.program, name.c_str());
            glUniformMatrix4fv(boneTransform, 1, GL_TRUE, (const GLfloat*) Transforms2[i]);
        }

        glm::mat4 pikachu_mod2 = glm::mat4(1.0f);
        pikachu_mod2 = glm::translate(pikachu_mod2, glm::vec3(1.4f, -0.4f, 0.0f)); // В ручную забили
        pikachu_mod2 = glm::scale(pikachu_mod2, glm::vec3(0.02, 0.02, 0.02));
        pikachu_mod2 = glm::rotate(pikachu_mod2, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        pikachu_mod2 = glm::rotate(pikachu_mod2, glm::radians(45.0f), glm::vec3(0.0f, -0.0f, -1.0f));

        shader.set_mat4_uniform("model", pikachu_mod2);
        slowpoke.Render(shader);


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
