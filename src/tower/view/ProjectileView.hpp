#pragma once
#include <SFML/Graphics.hpp>
#include "../logic/Projectile.hpp"
#include "../../ressource/RessourceManager.hpp"

class ProjectileView {
public:
    ProjectileView(const Projectile& logique);

    void update(const Projectile& logique);
    void draw(sf::RenderWindow& window) const;

private:
    sf::Sprite m_sprite;

    std::string choisirTexture(const std::string& type, int niveau);
    float       choisirEchelle(const std::string& type, int niveau);
};
