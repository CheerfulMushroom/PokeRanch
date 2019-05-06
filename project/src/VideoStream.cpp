#include <iostream>
#include <VideoStream.h>
#include <GLFW/glfw3.h>
#include <GL/glew.h>
#include <ShaderProgram.h>
#include <opencv2/opencv.hpp>


VideoStream::VideoStream(int width, int height) : window_width(width), window_height(height) {
    context_init();
}



bool VideoStream::context_init() {
    glfwInit();

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);


    window = glfwCreateWindow(window_width, window_height, "Hello", nullptr, nullptr);
    if (window == nullptr) {
        std::cout << "Failed to create window" << std::endl;
        glfwTerminate();
        return 1;
    }

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cout << "Failed to init GLEW" << std::endl;
        return 1;
    }


    glViewport(0, 0, window_width, window_height);

    //glfwSetKeyCallback(window, key_callback);
}

bool VideoStream::configure_VAO() {
    //GLuint texture;
    glGenTextures(1, &texture);

    glBindTexture(GL_TEXTURE_2D, texture);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);


    GLfloat vertices[] = {
         1.0f,  1.0f, 0.0f, 1.0f, 1.0f, 
         1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
        -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
        -1.0f,  1.0f, 0.0f, 0.0f, 1.0f
    };

    GLuint indices[] = {
        0, 1, 3,
        1, 2, 3
    };

    //GLuint EBO;
    glGenBuffers(1, &EBO);

    //GLuint VBO;
    glGenBuffers(1, &VBO);

    ShaderProgram shader("project/src/v_shader.txt", "project/src/f_shader.txt");


    

    //GLuint VAO;
    glGenVertexArrays(1, &VAO);

    glBindVertexArray(VAO);



    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) 0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*) (3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    


    shader.use();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(glGetUniformLocation(shader.program, "ourTexture1"), 0);


    glBindVertexArray(0);
    glfwSwapInterval(1);

}


/*void VideoStream::key_callback(int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        glfwSetWindowShouldClose(window, GL_TRUE);
    }
} */


void VideoStream::lock_frame_rate(double frame_rate) {
    double allowed_frame_time = 1.0 / frame_rate;  // Продолжительность одного кадра

    frame_end_time = glfwGetTime();
    double frame_draw_time = frame_end_time - frame_start_time;

    double sleep_time = 0.0;

    if (frame_draw_time < allowed_frame_time) {
        sleep_time = allowed_frame_time - frame_draw_time;
        usleep(1000000 * sleep_time);
    }
}

void VideoStream::draw_video_frame() {

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glBindTexture(GL_TEXTURE_2D, texture);
    
    //cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);
    cv::flip(frame, frame, 0);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, frame.cols, frame.rows, 0, GL_BGR, GL_UNSIGNED_BYTE, frame.ptr());
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    
    glBindVertexArray(0);
}
