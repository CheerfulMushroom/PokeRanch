#ifndef PREP_POKEMON_RANCH_BUTTON_H
#define PREP_POKEMON_RANCH_BUTTON_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

class Button {
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



#endif //PREP_POKEMON_RANCH_BUTTON_H
