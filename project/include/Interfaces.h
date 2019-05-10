#ifndef PREP_POKEMON_RANCH_INTERFACES_H
#define PREP_POKEMON_RANCH_INTERFACES_H

class Renderable {
public:
    virtual void render() = 0;
};

class Executable {
public:
    virtual void exec() = 0;
};

class Updatable {
public:
    virtual void update() = 0;
};


#endif //PREP_POKEMON_RANCH_INTERFACES_H
