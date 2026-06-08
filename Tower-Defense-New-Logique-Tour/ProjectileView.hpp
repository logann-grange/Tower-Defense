#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class ProjectileView {
private:
    sf::Sprite m_sprite;

    void chargerTexturesEtEchelle(const std::string& typeTour, int niveau);

public:
    ProjectileView(sf::Vector2f positionInitiale, const std::string& typeTour, int niveau);
    
    void synchroniser(sf::Vector2f nouvellePosition, float angleDegres);
    void draw(sf::RenderWindow &window) const;
};