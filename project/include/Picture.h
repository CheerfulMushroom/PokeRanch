#ifndef PREP_POKEMON_RANCH_PICTURE_H
#define PREP_POKEMON_RANCH_PICTURE_H

#include "Interfaces.h"

class Picture: public Renderable {
public:
    Picture(GLfloat x, GLfloat y,
            GLfloat x_size, GLfloat y_size,
            std::string path_to_file);
    ~Picture() override;

    void render() override;

private:
    ShaderProgram shader;
    GLuint VAO;
    GLuint VBO;
    GLuint EBO;
    GLuint texture;

};


#endif //PREP_POKEMON_RANCH_PICTURE_H
