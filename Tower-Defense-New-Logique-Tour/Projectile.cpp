#include "../include/Projectile.hpp"
#include "../include/ProjectileView.hpp"
#include "mob/Monster.hpp" 
#include <cmath>

Projectile::Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string &typeTour, int niveauTour)
    : m_cible(cible), m_positionActuelle(positionDepart), m_degats(degats),
      m_typeTour(typeTour), m_niveau(niveauTour), m_detruit(false)
{
    // Décalage de 16 pixels vers le haut pour simuler une sortie de canon réaliste depuis la tête de la tour
    m_positionActuelle.y -= 16.f; 
    
    // Instanciation de la vue graphique associée dans son fichier dédié
    m_view = std::make_unique<ProjectileView>(m_positionActuelle, m_typeTour, m_niveau);
}

Projectile::~Projectile() = default;

void Projectile::update(float deltaTime) {
    if (m_detruit) return;

    // SÉCURITÉ : Si la cible disparaît brusquement ou meurt, le projectile s'autodétruit
    if (!m_cible || m_cible->isDead()) { 
        m_detruit = true; 
        return; 
    }

    // Acquisition de la position précise du monstre (ajustée au centre du corps)
    sf::Vector2f posCible = m_cible->getPosition();
    posCible.y -= 24.f; 

    // Vecteur de direction mathématique
    sf::Vector2f direction = posCible - m_positionActuelle;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // DÉTECTION DE COLLISION (Rayon d'impact de 10 pixels)
    if (distance <= 10.f) {
        m_cible->takeDamage(m_degats); // Application des points de dégâts à la logique du monstre
        m_detruit = true;              // Demande de suppression immédiate
        return;
    }

    // DÉPLACEMENT RECTILIGNE ET HOMOGÈNE (Indépendant des variations de FPS grâce au deltaTime)
    sf::Vector2f directionNormalisee = direction / distance;
    m_positionActuelle += directionNormalisee * m_vitesse * deltaTime;
    
    // CALCUL DE L'ANGLE DE ROTATION POUR LE VISUEL (Trigonométrie standard : atan2)
    float const PI = 3.14159265f;
    float angle = std::atan2(direction.y, direction.x) * 180.f / PI;
    
    // TRANSMISSION DES COORDONNÉES À LA VUE POUR SYNCHRONISATION DU SPRITE
    m_view->synchroniser(m_positionActuelle, angle);
}
