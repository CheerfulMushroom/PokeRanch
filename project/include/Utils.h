#ifndef PREP_POKEMON_RANCH_UTILS_H
#define PREP_POKEMON_RANCH_UTILS_H

#include <opencv2/opencv.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "ShaderProgram.h"

bool get_path_by_id(int pok_id, std::string &model_path);

void mat_to_texture(GLuint texture, cv::Mat pic, bool with_alpha);

void configure_VAO(std::vector<GLfloat> vertices,
                   std::vector<GLuint> indices,
                   GLuint *VAO,
                   GLuint *VBO,
                   GLuint *EBO,
                   GLuint *texture,
                   ShaderProgram *shader);


#endif //PREP_POKEMON_RANCH_UTILS_H
