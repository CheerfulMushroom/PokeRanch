#ifndef PROJECT_INCLUDE_SHADER_HPP_
#define PROJECT_INCLUDE_SHADER_HPP_

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <GL/glew.h>

class ShaderProgram {
 public:
    ShaderProgram(const GLchar* vertices_shader_path, const GLchar* fragment_shader_path);
    ShaderProgram() = default;


    GLuint program;
    void use();

    void set_bool_uniform(const std::string &name, bool value) const;

    void set_int_uniform(const std::string &name, int value) const;

    void set_float_uniform(const std::string &name, float value) const;

    void set_vec2_uniform(const std::string &name, const glm::vec2 &value) const;

    void set_vec3_uniform(const std::string &name, const glm::vec3 &value) const;

    void set_vec4_uniform(const std::string &name, const glm::vec4 &value) const;

    void set_mat2_uniform(const std::string &name, const glm::mat2 &value) const;

    void set_mat3_uniform(const std::string &name, const glm::mat3 &value) const;

    void set_mat4_uniform(const std::string &name, const glm::mat4 &value) const;

    void set_bone_transform(const int index, const glm::mat4 &value) const;

};

#endif  // PROJECT_INCLUDE_SHADER_HPP_
