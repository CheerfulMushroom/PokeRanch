#ifndef PREP_POKEMON_RANCH_BUTTON_H
#define PREP_POKEMON_RANCH_BUTTON_H

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <functional>

class GameState;

class BaseButton {
public:
    virtual void render() = 0;

    virtual bool is_pointed() = 0;

    virtual void exec() = 0;
};

class Button : public BaseButton {
public:
    Button(GameState *state, GLfloat x, GLfloat y, GLfloat x_size, GLfloat y_size, std::function<void()> to_exec);

    ~Button();

    void render() override;

    bool is_pointed() override;

    void exec() override;

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
