#pragma once
#include "Projectile.hpp"

class ProjectileGlace : public Projectile {
public:
    ProjectileGlace(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible,
                    int degats, int niveauTour)
        : Projectile(positionDepart, cible, degats, "Glace", niveauTour)
    {}
};
