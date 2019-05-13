#include <iostream>
#include <ShaderProgram.h>
#include <opencv2/opencv.hpp>
#include <GLFW/glfw3.h>
#include <opencv2/highgui.hpp>
#include <GameWindow.h>
#include <Model.h>
#include <Camera.h>

#define FPS 25

int main() {

    GameWindow screen(800, 600, 25);

    glEnable(GL_DEPTH_TEST);

    Camera camera(glm::vec3(0.0f, 0.0f, 5.0f));

    ShaderProgram shader("project/src/v_model_shader.txt", "project/src/f_model_shader.txt");

    Model pikachu_model("models/pikachu.dae");

    while (!glfwWindowShouldClose(screen.window))
    {
        screen.frame_start_time = glfwGetTime();

        glClearColor(0.5f, 0.4f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        shader.use();

        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float) 800 / (float) 600, 0.1f, 100.0f);
        
        glm::mat4 view = camera.GetViewMatrix();

        shader.set_mat4_uniform("projection", projection);
        shader.set_mat4_uniform("view", view);

        glm::mat4 pikachu_mod = glm::mat4(1.0f);
        pikachu_mod = glm::translate(pikachu_mod, glm::vec3(0.5f, -0.4f, 0.0f));
        pikachu_mod = glm::scale(pikachu_mod, glm::vec3(0.05, 0.05, 0.05));
        pikachu_mod = glm::rotate(pikachu_mod, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));      

        shader.set_mat4_uniform("model", pikachu_mod);
        pikachu_model.render(shader);

        glfwSwapBuffers(screen.window);
        glfwPollEvents();

        screen.lock_frame_rate();
    }

    glfwTerminate();
    return 0;
}
