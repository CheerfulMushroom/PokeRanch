#ifndef PREP_POKEMON_RANCH_UTILS_H
#define PREP_POKEMON_RANCH_UTILS_H

#include <opencv2/opencv.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>


void mat_to_texture(GLuint texture, cv::Mat pic, bool with_alpha);


#endif //PREP_POKEMON_RANCH_UTILS_H
