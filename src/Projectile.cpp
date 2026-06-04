#include "../include/Projectile.hpp"
#include "mob/Monster.hpp" // Adapté pour inclure ta classe de base Monster
#include <cmath>
#include <iostream>

Projectile::Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string& typeTour)
    : m_cible(cible), m_positionDepart(positionDepart), m_degats(degats), 
      m_typeTour(typeTour), m_detruit(false)
{
    // Configuration graphique de la ligne du laser
    m_positionDepart = positionDepart + sf::Vector2f(0.f, -55.f);
    // Configuration graphique de la ligne du laser
    m_laserLine.setFillColor(sf::Color(0, 240, 255, 220)); // Cyan éclatant semi-transparent
    
    // On cale l'origine sur (X=0, Y=Moitié de l'épaisseur) pour qu'il pivote depuis le sommet de la tour
    m_laserLine.setOrigin({0.f, 2.f}); // Épaisseur de 4 pixels, origine au milieu vertical (2.f)

    // Applique les dégâts immédiatement au monstre dès que le laser s'allume
    if (m_cible && !m_cible->isDead()) {
        m_cible->takeDamage(m_degats);
    }
}

void Projectile::update(float deltaTime) {
    if (m_detruit) return;

    m_dureeVieActuelle += deltaTime;
    if (m_dureeVieActuelle >= m_dureeVieMax || !m_cible || m_cible->isDead()) {
        m_detruit = true;
        return;
    }

    // --- CALCUL GÉOMÉTRIQUE DU RAYON LASER ---
    sf::Vector2f posCiblePixels = m_cible->getPosition();

    // On ajuste le point visé pour viser le torse du squelette plutôt que ses pieds
    // (Puisque son origine est à 52px de hauteur, on remonte le laser de 24 pixels)
    posCiblePixels.y -= 24.f;

    sf::Vector2f direction = posCiblePixels - m_positionDepart;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    m_laserLine.setPosition(m_positionDepart);

    // --- ⚡ EFFET VISUEL INTERESSANT : PULSATION D'ÉNERGIE ⚡ ---
    // On calcule une épaisseur oscillant entre 2 et 6 pixels en fonction du temps
    float pulsation = std::sin(m_dureeVieActuelle * 50.f) * 2.f; 
    float epaisseurDynamique = 4.f + pulsation; 

    // On ajuste l'origine verticale pour que l'épaisseur reste centrée
    m_laserLine.setOrigin({0.f, epaisseurDynamique / 2.f});
    m_laserLine.setSize({distance, epaisseurDynamique});

    // Optionnel : On peut faire scintiller la couleur du laser (plus ou moins transparent)
    int alpha = 180 + static_cast<int>(std::sin(m_dureeVieActuelle * 100.f) * 50);
    m_laserLine.setFillColor(sf::Color(0, 230, 255, alpha));

    // Calcul de l'angle
    float angle = std::atan2(direction.y, direction.x) * 180.f / 3.14159265f;
    m_laserLine.setRotation(sf::degrees(angle)); 
}

void Projectile::draw(sf::RenderWindow& window) const {
    if (!m_detruit) {
        window.draw(m_laserLine);
    }
}