#ifndef PREP_POKEMON_RANCH_STATES_H
#define PREP_POKEMON_RANCH_STATES_H

#include <bits/unique_ptr.h>
#include <vector>

#include "VideoStream.h"

class Game;
class GameElement;
class Renderable;
class Interactable;
class Updatable;

class GameState {
public:
    explicit GameState(Game *game_object) : game(game_object) {};
    virtual ~GameState() = default;
    virtual void load_scene() = 0;
    virtual void render_game() = 0;
    virtual void update_game() = 0;

    Game* get_game();

    std::vector<std::unique_ptr<GameElement>> game_elements;
    std::vector<Renderable*> to_render;
    std::vector<Interactable*> to_exec;
    std::vector<Updatable*> to_update;


protected:
    Game *game;
};


class MenuState : public GameState {
public:
    explicit MenuState(Game *game_object);
    void load_scene() override;
    void render_game() override;
    void update_game() override;
};


class PauseState : public GameState {
public:
    explicit PauseState(Game *game_object);
    void load_scene() override;
    void render_game() override;
    void update_game() override;
};


class FarmState : public GameState {
public:
    explicit FarmState(Game *game_object);
    void load_scene() override;
    void render_game() override;
    void update_game() override;

    VideoStream video_stream;
};


class PokedexState : public GameState {
public:
    explicit PokedexState(Game *game_object);
    void load_scene() override;
    void render_game() override;
    void update_game() override;
};


#endif //PREP_POKEMON_RANCH_STATES_H
