#include "../include/Projectile.hpp"
#include "../include/ProjectileView.hpp"
#include "mob/Monster.hpp" 
#include <cmath>

Projectile::Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string &typeTour, int niveauTour)
    : m_cible(cible), m_positionActuelle(positionDepart), m_degats(degats),
      m_typeTour(typeTour), m_niveau(niveauTour), m_detruit(false)
{
    m_positionActuelle.y -= 16.f; // Décalage de tir
    
    // Instanciation de la vue dans son fichier dédié
    m_view = std::make_unique<ProjectileView>(m_positionActuelle, m_typeTour, m_niveau);
}

Projectile::~Projectile() = default;

void Projectile::update(float deltaTime) {
    if (m_detruit) return;

    if (!m_cible || m_cible->isDead()) { m_detruit = true; return; }

    sf::Vector2f posCible = m_cible->getPosition();
    posCible.y -= 24.f; 

    sf::Vector2f direction = posCible - m_positionActuelle;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance <= 10.f) {
        m_cible->takeDamage(m_degats); 
        m_detruit = true;             
        return;
    }

    sf::Vector2f directionNormalisee = direction / distance;
    m_positionActuelle += directionNormalisee * m_vitesse * deltaTime;
    
    // Transmission des calculs mathématiques purs à la vue pour affichage
    float const PI = 3.14159265f;
    float angle = std::atan2(direction.y, direction.x) * 180.f / PI;
    
    m_view->synchroniser(m_positionActuelle, angle);
}