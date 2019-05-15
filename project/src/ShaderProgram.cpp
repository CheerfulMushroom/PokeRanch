#include <ShaderProgram.h>
#include <cstring>

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
    glShaderSource(vertices_shader, 1, &v_shader_source, NULL);
    glCompileShader(vertices_shader);


    fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &f_shader_source, NULL);
    glCompileShader(vertices_shader);


    program = glCreateProgram();
    glAttachShader(program, vertices_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);

    glDeleteShader(vertices_shader);
    glDeleteShader(fragment_shader);
}

void ShaderProgram::use() {
    glUseProgram(program);
}


void ShaderProgram::set_bool_uniform(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), (int) value);
}

void ShaderProgram::set_int_uniform(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(program, name.c_str()), value);

}

void ShaderProgram::set_float_uniform(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(program, name.c_str()), value);

}

void ShaderProgram::set_vec2_uniform(const std::string &name, const glm::vec2 &value) const {
    glUniform2fv(glGetUniformLocation(program, name.c_str()), 1,  &value[0]);
}

void ShaderProgram::set_vec3_uniform(const std::string &name, const glm::vec3 &value) const {
    glUniform3fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void ShaderProgram::set_vec4_uniform(const std::string &name, const glm::vec4 &value) const {
    glUniform4fv(glGetUniformLocation(program, name.c_str()), 1, &value[0]);
}

void ShaderProgram::set_mat2_uniform(const std::string &name, const glm::mat2 &value) const {
    glUniformMatrix2fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::set_mat3_uniform(const std::string &name, const glm::mat3 &value) const {
    glUniformMatrix3fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void ShaderProgram::set_mat4_uniform(const std::string &name, const glm::mat4 &value) const {
    glUniformMatrix4fv(glGetUniformLocation(program, name.c_str()), 1, GL_FALSE, &value[0][0]);
}
