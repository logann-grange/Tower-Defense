#include "Projectile.hpp"
#include "../../mob/Monster.hpp"
#include <cmath>

Projectile::Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible,
                       int degats, const std::string& typeTour, int niveauTour)
    : m_cible(cible), m_positionActuelle(positionDepart),
      m_degats(degats), m_typeTour(typeTour), m_niveau(niveauTour)
{
    m_positionActuelle.y -= 16.f; // Décalage au sommet de la tour
}

void Projectile::update(float deltaTime) {
    if (m_detruit) return;

    if (!m_cible || m_cible->estMort()) {
        m_detruit = true;
        return;
    }

    sf::Vector2f posCible = m_cible->getPosition();
    posCible.y -= 24.f; // Ciblage du torse

    sf::Vector2f direction = posCible - m_positionActuelle;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Collision
    if (distance <= 10.f) {
        m_cible->subirDegats(static_cast<float>(m_degats), m_typeTour);
        m_detruit = true;
        return;
    }

    // Déplacement
    sf::Vector2f dirNorm = direction / distance;
    m_positionActuelle += dirNorm * m_vitesse * deltaTime;

    // Angle pour la vue
    const float PI = 3.14159265f;
    m_angle = std::atan2(direction.y, direction.x) * 180.f / PI;
}
