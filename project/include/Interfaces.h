#ifndef PREP_POKEMON_RANCH_INTERFACES_H
#define PREP_POKEMON_RANCH_INTERFACES_H

/** Здесь храняться интерфейсы, которые должны иметь классы,
 *  которые требуют рендера, обновлений, или с которыми может взаимодействовать пользователь.
 *  (Интерфейс - абсолютно абстрактный класс, который гарантирует наличие у дочернего класса определённых методов.)
 *
 *  Инициализация интерфейса  требуется для того, чтобы объект класса
 *  был добавлен как GameElement в вектор игровых элементов настоящего игрового состояния.
 *
 *  В зависимости от интерфейсов, данных классу, указатель на объект этого класса будет добавлен в соответсвующие
 *  вектора состояний: to_render (для Renderable), to_exec (для Interactable), to_update (для Updatable).
 *  Это требуется, например, для того чтобы отрисовывать все объекты, которые могут быть отрисованы.
 *  Так отрисовываются объекты класса Button и VideoStream в состоянии RanchState
 */


#include "Game.h"

class GameElement {
public:
    virtual ~GameElement() = default;
};

class Renderable : virtual public GameElement {
public:
    Renderable();

    // Отвечает за рендер
    virtual void render() = 0;
};

class Interactable : virtual public GameElement {
public:
    Interactable();

    // Говорит, если пользователь , курсором указывает на объект
    virtual bool is_pointed_at() = 0;
    // Возвращает расстояние до объекта
    virtual double get_distance() = 0;
    // Выполнение какого-либо действия
    virtual void exec() = 0;
};

class Updatable : virtual public GameElement {
public:
    Updatable();

    // Обновляет данные объекта
    virtual void update() = 0;
};


#endif //PREP_POKEMON_RANCH_INTERFACES_H
