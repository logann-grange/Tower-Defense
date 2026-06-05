#pragma once
#ifndef CASTLEVIEW_HPP
#define CASTLEVIEW_HPP
#include <SFML/Graphics.hpp>
#include "../logic/Castle.hpp"
#include "../../ressource/RessourceManager.hpp"

class CastleView {
public:
    CastleView();
    void update(const Castle& logique);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite m_sprite;
    sf::RectangleShape m_barreFond;
    sf::RectangleShape m_barrePV;
};









#endif // CASTLEVIEW_HPP