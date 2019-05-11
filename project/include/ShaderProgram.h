#ifndef PROJECT_INCLUDE_SHADER_HPP_
#define PROJECT_INCLUDE_SHADER_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

#include <GL/glew.h>

class ShaderProgram {
 public:
    ShaderProgram(const GLchar* vertices_shader_path, const GLchar* fragment_shader_path);
    ShaderProgram()= default;

    // Использовать шейдер. Нужно вызывать перед рендером
    void use();

    // Геттеры
    GLuint get_program();

private:
    GLuint program;
};

#endif
