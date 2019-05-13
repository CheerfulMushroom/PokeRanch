#ifndef PREP_POKEMON_RANCH_BUTTON_H
#define PREP_POKEMON_RANCH_BUTTON_H

#include <opencv2/opencv.hpp>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

#include "Interfaces.h"
#include "ShaderProgram.h"

class GameState;

//TODO(me): add destructor
class Button : public Renderable, public Interactable {
public:
    Button(GameState *state,
                   GLfloat x, GLfloat y,
                   GLfloat x_size, GLfloat y_size,
                   std::function<void()> to_exec,
                   std::string path_to_icon = "project/icons/default.png");
    ~Button();
    void render() override;
    bool is_triggered() override;
    void exec() override;

    void mat_to_texture();

private:
    GameState *state;
    GLfloat x;
    GLfloat y;
    GLfloat x_size;
    GLfloat y_size;
    std::function<void()> to_exec;
    cv::Mat icon;

    ShaderProgram shader;
    GLuint VAO, VBO, EBO, texture;

};


#endif //PREP_POKEMON_RANCH_BUTTON_H
