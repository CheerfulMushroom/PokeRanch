#ifndef PREP_POKEMON_RANCH_BUTTON_H
#define PREP_POKEMON_RANCH_BUTTON_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

#include "Interfaces.h"

class GameState;

class Button : public Renderable, public Interactable {
public:
    Button(GameState *state, GLfloat x, GLfloat y, GLfloat x_size, GLfloat y_size, std::function<void()> to_exec);
    ~Button();
    void render() override;
    bool is_triggered() override;
    void exec();

private:
    GameState *state;
    GLfloat x;
    GLfloat y;
    GLfloat x_size;
    GLfloat y_size;
    std::function<void()> to_exec;
    GLuint VAO, VBO;
};


#endif //PREP_POKEMON_RANCH_BUTTON_H
