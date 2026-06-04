#include "../include/Projectile.hpp"
#include "mob/Monster.hpp" 
#include <cmath>
#include <iostream>

Projectile::Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string &typeTour, ModeTir mode)
    : m_cible(cible), m_positionDepart(positionDepart), m_degats(degats),
      m_typeTour(typeTour), m_mode(mode), m_detruit(false)
{
    // Hauteur de tir
    if (m_typeTour == "Glace") {
        m_positionDepart = positionDepart + sf::Vector2f(0.f, -45.f); 
    } else {
        m_positionDepart = positionDepart + sf::Vector2f(0.f, -16.f); 
    }

    // Couleurs
    if (m_typeTour == "Glace") m_laserLine.setFillColor(sf::Color(0, 240, 255, 220));
    else if (m_typeTour == "Feu") m_laserLine.setFillColor(sf::Color(255, 60, 0, 220));
    else if (m_typeTour == "Foudre") m_laserLine.setFillColor(sf::Color(255, 230, 0, 220));
    else m_laserLine.setFillColor(sf::Color(255, 255, 255, 220));

    m_laserLine.setOrigin({0.f, 2.f}); 

    // Premier coup direct à l'allumage du laser
    if (m_cible && !m_cible->isDead()) {
        m_cible->takeDamage(m_degats);
    }
}

void Projectile::update(float deltaTime) {
    if (m_detruit) return;

    m_dureeVieActuelle += deltaTime;

    // 💡 LOGIQUE DYNAMIQUE DU CYCLE DE VIE
    if (!m_cible || m_cible->isDead()) {
        m_detruit = true;
        return;
    }

    // Si on est en MODE ESPACÉ, le laser s'autodétruit après sa durée max (0.12s)
    if (m_mode == ModeTir::Espace && m_dureeVieActuelle >= m_dureeVieMax) {
        m_detruit = true;
        return;
    }

    // --- CALCUL GÉOMÉTRIQUE DE SUIVI ---
    sf::Vector2f posCiblePixels = m_cible->getPosition();
    posCiblePixels.y -= 24.f; 

    sf::Vector2f direction = posCiblePixels - m_positionDepart;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Sécurité de portée (si le monstre s'enfuit du rayon)
    if (distance > 165.f) {
        m_detruit = true;
        return;
    }

    m_laserLine.setPosition(m_positionDepart);
    m_laserLine.setSize({distance, 4.f});

    // Effet visuel (scintillement rapide pour le mode espacé, plus doux pour le continu)
    float vitessePulsation = (m_mode == ModeTir::Espace) ? 100.f : 40.f;
    int alpha = 180 + static_cast<int>(std::sin(m_dureeVieActuelle * vitessePulsation) * 50);
    
    if (m_typeTour == "Feu") m_laserLine.setFillColor(sf::Color(255, 50, 0, alpha));
    else m_laserLine.setFillColor(sf::Color(0, 230, 255, alpha));

    float const PI = 3.14159265f;
    float angle = std::atan2(direction.y, direction.x) * 180.f / PI;
    m_laserLine.setRotation(sf::degrees(angle)); 

    // 💡 SÉCURITÉ POUR LE MODE CONTINU : Appliquer des dégâts légers au fil du temps
    if (m_mode == ModeTir::Continu) {
        static float chronoDegats = 0.f;
        chronoDegats += deltaTime;
        if (chronoDegats >= 0.25f) {
            m_cible->takeDamage(m_degats);
            chronoDegats = 0.f;
        }
    }
}

void Projectile::draw(sf::RenderWindow &window) const {
    if (!m_detruit) {
        window.draw(m_laserLine);
    }
}