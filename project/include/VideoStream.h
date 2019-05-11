#ifndef PROJECT_INCLUDE_VIDEOSTREAM_H_
#define PROJECT_INCLUDE_VIDEOSTREAM_H_

#include <iostream>
#include <unistd.h>
#include <opencv2/opencv.hpp>

<<<<<<< HEAD
class VideoStream {
 public:
    VideoStream();
=======

#include <GL/glew.h>
#include <GLFW/glfw3.h>


#include "ShaderProgram.h"
#include "VideoStream.h"
#include "Interfaces.h"
>>>>>>> fbe8afb635d54f514c813de684e09135e2a00462


<<<<<<< HEAD
    bool configure_VAO();

=======
class VideoStream: public Renderable {
 public:
    VideoStream(int cam_index);
    ~VideoStream();

    void render();
    
    bool configure_VAO();


 private:
    void mat_to_texture();

    ShaderProgram shader;
    cv::VideoCapture cam;
>>>>>>> fbe8afb635d54f514c813de684e09135e2a00462
    cv::Mat frame;
    GLuint texture;
    GLuint VBO;
    GLuint VAO;
    GLuint EBO;
<<<<<<< HEAD


 private:
    void mat_to_texture();
    
=======
>>>>>>> fbe8afb635d54f514c813de684e09135e2a00462
};

#endif // PROJECT_INCLUDE_VIDEOSTREAM_H_
