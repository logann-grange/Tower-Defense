#include "../include/Tour.hpp"
#include "../include/TourView.hpp" // Besoin de la vue pour l'initialiser et déclencher le son
#include "mob/Monster.hpp"
#include "Projectile.hpp"
#include <cmath>
#include <iostream>

Tour::Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, int niveau, sf::Vector2f pos, const std::string &texturePath)
    : m_id(id), m_atk(atk), m_valeur(valeur), m_type(type), m_portee(portee),
      m_vitesseAtk(vitesseAtk), m_niveau(niveau), m_position(pos), m_valeurEvo(valeur * 0.6f),
      m_tempsDepuisDerniereAtk(0.0f),
      m_view(std::make_unique<TourView>(pos, type, texturePath)) // Allocation de la vue séparée
{}

Tour::~Tour() = default;

void Tour::update(float deltaTime, const std::vector<std::shared_ptr<Monster>> &listeEnemis, std::vector<std::unique_ptr<Projectile>> &listeProjectiles)
{
    m_tempsDepuisDerniereAtk += deltaTime;
    float porteeEnPixels = m_portee * 16.0f;
    float porteeEnPixelsSq = std::pow(porteeEnPixels, 2);

    if (m_cible) {
        if (m_cible->isDead()) { m_cible = nullptr; }
        else {
            sf::Vector2f posCible = m_cible->getPosition();
            float distSq = std::pow(posCible.x - m_position.x, 2) + std::pow(posCible.y - m_position.y, 2);
            if (distSq > porteeEnPixelsSq) m_cible = nullptr; 
        }
    }

    if (!m_cible) {
        float distanceMinSq = porteeEnPixelsSq; 
        for (const auto &enemi : listeEnemis) {
            if (enemi && !enemi->isDead()) {
                sf::Vector2f posEnemi = enemi->getPosition();
                float distSq = std::pow(posEnemi.x - m_position.x, 2) + std::pow(posEnemi.y - m_position.y, 2);
                if (distSq <= distanceMinSq) { m_cible = enemi; distanceMinSq = distSq; }
            }
        }
    }

    if (m_cible && m_tempsDepuisDerniereAtk >= m_vitesseAtk) {
        attaquer(m_cible, listeProjectiles);
        m_tempsDepuisDerniereAtk = 0.0f; 
    }
}

void Tour::attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>>& listeProjectiles)
{
    if (cible) {
        listeProjectiles.push_back(std::make_unique<Projectile>(m_position, cible, m_atk, m_type, m_niveau));
        m_view->jouerSonTir(); // Déclenchement du son via l'objet graphique externe
    }
}

void Tour::upgrade() {
    if (m_niveau < 3) { m_niveau++; m_atk += 15; }
}

void Tour::tenterAmelioration(int& orJoueur) {
    int coutUpgrade = m_niveau * 50; 
    if (orJoueur >= coutUpgrade) {
        orJoueur -= coutUpgrade; 
        m_niveau++;              
        m_atk = static_cast<int>(m_atk * 1.30f);        
        m_portee += 25;                                 
        m_vitesseAtk *= 0.85f;                          
    }
}