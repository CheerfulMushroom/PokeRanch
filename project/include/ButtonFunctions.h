#ifndef PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H
#define PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H

/** Здесь храняться функции, которые записываются в кнопки
 *  Функции не должны принимать аргументов
 */

#include <GL/glew.h>

#include "Game.h"
#include "States.h"


void change_to_ranch();

void change_to_pokedex(int pokemon_id);

void change_to_pause();

double test_movement(double t);


#endif //PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H
