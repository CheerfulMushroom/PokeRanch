#ifndef PREP_POKEMON_RANCH_RENDEROBJECT_H
#define PREP_POKEMON_RANCH_RENDEROBJECT_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "gameObject.h"


class RenderObject {
public:
    virtual void render() = 0;

private:
    GameObject *game;
};

class Button : public RenderObject {
public:
    Button(GLFWwindow *window, GLfloat x, GLfloat y, GLfloat x_size, GLfloat y_size);

    ~Button();

    void render();

    bool is_pointed();

private:
    GLFWwindow *window;
    GLfloat x;
    GLfloat y;
    GLfloat x_size;
    GLfloat y_size;
    GLuint VAO, VBO;

};


#endif //PREP_POKEMON_RANCH_RENDEROBJECT_H
