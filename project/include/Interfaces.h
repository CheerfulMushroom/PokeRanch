#ifndef PREP_POKEMON_RANCH_INTERFACES_H
#define PREP_POKEMON_RANCH_INTERFACES_H

#include "Game.h"

class GameElement {
};

class Renderable : virtual public GameElement {
public:
    Renderable();
    virtual void render() = 0;
};

class Interactable : virtual public GameElement {
public:
    Interactable();
    virtual bool triggered() = 0;
    virtual void exec() = 0;
};

class Updatable : virtual public GameElement {
public:
    Updatable();
    virtual void update() = 0;
};


#endif //PREP_POKEMON_RANCH_INTERFACES_H
