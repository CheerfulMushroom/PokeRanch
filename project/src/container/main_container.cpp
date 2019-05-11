#include <iostream>
#include <GameWindow.h>
#include <opencv2/opencv.hpp>
#include <container.h>

#define FPS 25

int main() {

    GameWindow screen(600, 600, FPS);

    cv::Mat pepe = cv::imread("pepe.png");
    cv::Mat box = cv::imread("box.jpg");

    GLuint texture_pepe;
    GLuint texture_box;
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;

    glGenTextures(1, &texture_pepe);
    glGenTextures(1, &texture_box);
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &EBO);

    mat_to_texture(pepe, texture_pepe);
    mat_to_texture(box, texture_box);

    configure_VAO(texture_pepe, texture_box, VBO, VAO, EBO);


    while (!glfwWindowShouldClose(screen.window)) {
        screen.frame_start_time = glfwGetTime();

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glfwPollEvents();
    
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
     
        glfwSwapBuffers(screen.window);
        screen.lock_frame_rate();
    }


    glfwTerminate();
    return 0;
}

    


    
    
