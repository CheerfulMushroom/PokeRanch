#ifndef PREP_POKEMON_RANCH_STATES_H
#define PREP_POKEMON_RANCH_STATES_H

#include <bits/unique_ptr.h>
#include <vector>

#include "Button.h"

class GameObject;

class GameState {
public:
    explicit GameState(GameObject *game_object) : game(game_object) {};

    virtual ~GameState() = default;

    virtual void render_game() = 0;

    virtual void update_game() = 0;

    std::vector<std::unique_ptr<BaseButton>> buttons;

    GameObject *game;
};

class MenuState : public GameState {
public:
    explicit MenuState(GameObject *game_object);

    void render_game() override;

    void update_game() override;
};

class PauseState : public GameState {
public:
    explicit PauseState(GameObject *game_object);

    void render_game() override;

    void update_game() override;
};

class FarmState : public GameState {
public:
    explicit FarmState(GameObject *game_object);

    void render_game() override;

    void update_game() override;
};

class PokedexState : public GameState {
public:
    explicit PokedexState(GameObject *game_object);

    void render_game() override;

    void update_game() override;
};


#endif //PREP_POKEMON_RANCH_STATES_H
