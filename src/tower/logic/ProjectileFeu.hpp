#pragma once
#include "Projectile.hpp"

class ProjectileFeu : public Projectile {
public:
    ProjectileFeu(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible,
                  int degats, int niveauTour)
        : Projectile(positionDepart, cible, degats, "Feu", niveauTour)
    {}
};
