#include "../include/Projectile.hpp"
#include "mob/Monster.hpp" 
#include <cmath>
#include <iostream>

Projectile::Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string &typeTour)
    : m_cible(cible), m_positionDepart(positionDepart), m_degats(degats),
      m_typeTour(typeTour), m_detruit(false)
{
    // On applique le décalage de hauteur standard de -16 pixels (le haut de la tourelle)
    m_positionDepart = positionDepart + sf::Vector2f(0.f, -16.f);

    // Couleur initiale
    if (m_typeTour == "Feu") {
        m_laserLine.setFillColor(sf::Color(255, 60, 0, 220)); // Rouge
    } else {
        m_laserLine.setFillColor(sf::Color(0, 240, 255, 220)); // Cyan par défaut
    }
    
    m_laserLine.setOrigin({0.f, 2.f}); 

    // Premier coup direct à l'allumage du laser
    if (m_cible && !m_cible->isDead()) {
        m_cible->takeDamage(m_degats);
    }
}

void Projectile::update(float deltaTime) {
    if (m_detruit) return;

    m_dureeVieActuelle += deltaTime;

    // 💡 SÉCURITÉ : Le laser s'arrête UNIQUEMENT si le monstre meurt ou s'il n'existe plus
    // On supprime la condition limitative des 0.12 secondes !
    if (!m_cible || m_cible->isDead()) {
        m_detruit = true;
        return;
    }

    // Calcul de suivi de la cible en temps réel
    sf::Vector2f posCiblePixels = m_cible->getPosition();
    posCiblePixels.y -= 24.f; // Vise le torse

    sf::Vector2f direction = posCiblePixels - m_positionDepart;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Si le monstre sort de la portée de la tour (150 case de portée + marge), le laser se coupe
    if (distance > 165.f) {
        m_detruit = true;
        return;
    }

    m_laserLine.setPosition(m_positionDepart);
    m_laserLine.setSize({distance, 4.f});

    // 🎨 EFFET DE SCINTILLEMENT : Fait fluctuer la transparence (Alpha) pour un effet d'énergie instable
    int alpha = 180 + static_cast<int>(std::sin(m_dureeVieActuelle * 60.f) * 50);
    if (m_typeTour == "Feu") {
        m_laserLine.setFillColor(sf::Color(255, 50, 0, alpha));
    } else {
        m_laserLine.setFillColor(sf::Color(0, 230, 255, alpha));
    }

    // Rotation pour SFML 3
    float const PI = 3.14159265f;
    float angle = std::atan2(direction.y, direction.x) * 180.f / PI;
    m_laserLine.setRotation(sf::degrees(angle)); 
}

void Projectile::draw(sf::RenderWindow &window) const {
    if (!m_detruit) {
        window.draw(m_laserLine);
    }
}