#include <ShaderProgram.h>
#include <cstring>

#include <GL/glew.h>

#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const char *v_shader_path, const char *f_shader_path) {


    //  Получение исходного кода шейдеров из файла

    std::ifstream v_shader_file(v_shader_path);
    std::ifstream f_shader_file(f_shader_path);

    std::stringstream v_shader_stream;
    std::stringstream f_shader_stream;
    
    v_shader_stream << v_shader_file.rdbuf();
    f_shader_stream << f_shader_file.rdbuf();

    std::string v_shader_string(v_shader_stream.str());
    std::string f_shader_string(f_shader_stream.str());

    const char *v_shader_source = v_shader_string.c_str();
    const char *f_shader_source = f_shader_string.c_str();


    //  Компиляция и сборка шейдеров


    GLuint vertices_shader = 0;
    GLuint fragment_shader = 0;

    vertices_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertices_shader, 1, &v_shader_source, nullptr);
    glCompileShader(vertices_shader);

    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &f_shader_source, nullptr);
    glCompileShader(fragment_shader);

    // Проверка шейдеров и вывод лога в случае ошибки
    GLint success;
    GLchar infoLog[512];

    glGetShaderiv(vertices_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertices_shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragment_shader, 512, nullptr, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }

    program = glCreateProgram();
    glAttachShader(program, vertices_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertices_shader);
    glDeleteShader(fragment_shader);
};

void ShaderProgram::use() {
    glUseProgram(program);
}

GLuint ShaderProgram::get_program() {
    return program;
}
