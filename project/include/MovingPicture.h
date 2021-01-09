#ifndef PREP_POKEMON_RANCH_MOVINGPICTURE_H
#define PREP_POKEMON_RANCH_MOVINGPICTURE_H

#include <GL/glew.h>
#include <functional>

#include "Interfaces.h"


class MovingPicture : public Renderable, public Updatable {
public:
    MovingPicture(GLfloat x, GLfloat y,
                  GLfloat x_size, GLfloat y_size,
                  std::function<GLfloat(GLfloat)> x_movement,
                  std::function<GLfloat(GLfloat)> y_movement,
                  std::string path_to_file = "project/icons/default.png");

    ~MovingPicture() override;

    void render() override;
    void update() override;

private:
    GLfloat x;
    GLfloat y;
    GLfloat x_size;
    GLfloat y_size;
    std::function<double(double)> x_movement;
    std::function<double(double)> y_movement;
    ShaderProgram shader;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture;

};


#endif //PREP_POKEMON_RANCH_MOVINGPICTURE_H
