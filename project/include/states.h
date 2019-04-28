#ifndef PREP_POKEMON_RANCH_STATES_H
#define PREP_POKEMON_RANCH_STATES_H

#include <bits/unique_ptr.h>

class GameState {
public:
    explicit GameState(class GameObject *game_object) : game(game_object) {};

    virtual ~GameState() = default;

    virtual void render_game() = 0;

    virtual void update_game() = 0;

protected:
    class GameObject *game;
};

class MenuState : public GameState {
public:
    explicit MenuState(GameObject *game_object) : GameState(game_object) {};

    void render_game() override;

    void update_game() override;
};

class PauseState : public GameState {
public:
    explicit PauseState(GameObject *game_object) : GameState(game_object) {};

    void render_game() override;

    void update_game() override;
};

class FarmState : public GameState {
public:
    explicit FarmState(GameObject *game_object) : GameState(game_object) {};

    void render_game() override;

    void update_game() override;
};

class PokedexState : public GameState {
public:
    explicit PokedexState(GameObject *game_object) : GameState(game_object) {};

    void render_game() override;

    void update_game() override;
};

class GameObject {
public:
    void render_game() {
        state->render_game();
    }

    void update_game() {
        state->update_game();
    }

    void change_state(std::unique_ptr<GameState> state) {
        this->state = std::move(state);
    }

private:
    std::unique_ptr<GameState> state = std::make_unique<MenuState>(this);
};


#endif //PREP_POKEMON_RANCH_STATES_H
