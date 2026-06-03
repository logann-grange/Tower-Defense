#ifndef MONSTER_VIEW_HPP
#define MONSTER_VIEW_HPP

#include <SFML/Graphics.hpp>
#include "Monster.hpp"

class MonsterView {
public:
    virtual ~MonsterView() = default;

    // Fonctions virtuelles pures (égales à 0, donc pas de .cpp)
    virtual void update(float deltaTime, const Monster& logique) = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
};

#endif // MONSTER_VIEW_HPP