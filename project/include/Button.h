#ifndef PREP_POKEMON_RANCH_BUTTON_H
#define PREP_POKEMON_RANCH_BUTTON_H


#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

#include "Interfaces.h"
#include "ShaderProgram.h"

class GameState;

class Button : public Renderable, public Interactable {
public:
    Button(GameState *state,
                   GLfloat x, GLfloat y,
                   GLfloat x_size, GLfloat y_size,
                   std::function<void()> to_exec = nullptr,
                   std::string path_to_pic = "project/icons/default.png");
    ~Button() override;

    void render() override;
    bool is_pointed_at() override;
    double get_distance() override;
    void exec() override;

private:
    GameState *state;
    GLfloat x;
    GLfloat y;
    GLfloat x_size;
    GLfloat y_size;
    std::function<void()> to_exec;

    ShaderProgram shader;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture;

};


#endif //PREP_POKEMON_RANCH_BUTTON_H
