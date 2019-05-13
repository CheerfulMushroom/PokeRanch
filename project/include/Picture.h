#ifndef PREP_POKEMON_RANCH_PICTURE_H
#define PREP_POKEMON_RANCH_PICTURE_H

#include "Interfaces.h"

class Picture: public Renderable {
    Picture(GLfloat x, GLfloat y,
            GLfloat x_size, GLfloat y_size,
            std::string path_to_file);
    void render() override;
};


#endif //PREP_POKEMON_RANCH_PICTURE_H
