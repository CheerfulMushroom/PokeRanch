#ifndef PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H
#define PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H

/** Здесь храняться функции, которые записываются в кнопки **/

#include <GL/glew.h>

#include "Game.h"
#include "States.h"


void change_to_ranch();

void change_to_pokedex(int pokemon_id);

void change_to_pause();

void swap_cam();

void take_screenshot(int width, int height);

#endif //PREP_POKEMON_RANCH_BUTTONFUNCTIONS_H
