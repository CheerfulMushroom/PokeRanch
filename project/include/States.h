#ifndef PREP_POKEMON_RANCH_STATES_H
#define PREP_POKEMON_RANCH_STATES_H

/** Описывает состояния игры.
 * Каждое состояние хранит в себе элементы GameElement,
 * требуемые для его корректной работы (кнопки, фоны, видеопотоки)
 *
 * В векторе game_elements хранятся все игровые элементы
 * В векторе to_render хранятся элементы, которые требуется отрендерить
 * В векторе to_exec хранятся элементы, которые требуется проверять на взаимодействие с пользователем
 * В векторе to_update хранятся элементы, которые требуется обновлять
 *
 * После создания состояния требуется загрузить сцену (добавить в game_elements требуемые элементы)
 */


#include <bits/unique_ptr.h>
#include <vector>

#include "VideoStream.h"
#include "Camera.h"

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

    void add_element(std::unique_ptr<GameElement> el);

    // Геттеры
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


class RanchState : public GameState {
public:
    explicit RanchState(Game *game_object);
    void load_scene() override;
    void render_game() override;
    void update_game() override;

    Camera camera;
};


class PokedexState : public GameState {
public:
    explicit PokedexState(Game *game_object);
    void load_scene() override;
    void render_game() override;
    void update_game() override;

    Camera camera;
};


#endif //PREP_POKEMON_RANCH_STATES_H
