#include "../include/Tour.hpp"
#include "../include/TourView.hpp" // Inclusion requise pour instancier et piloter la vue
#include "mob/Monster.hpp"
#include "Projectile.hpp"
#include <cmath>
#include <iostream>

Tour::Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, int niveau, sf::Vector2f pos, const std::string &texturePath)
    : m_id(id), m_atk(atk), m_valeur(valeur), m_type(type), m_portee(portee),
      m_vitesseAtk(vitesseAtk), m_niveau(niveau), m_position(pos), m_valeurEvo(valeur * 0.6f),
      m_tempsDepuisDerniereAtk(0.0f),
      // On instancie l'objet graphique séparé en lui transmettant les infos dont il a besoin
      m_view(std::make_unique<TourView>(pos, type, texturePath)) 
{}

Tour::~Tour() = default; // Libère automatiquement le unique_ptr de la vue

void Tour::update(float deltaTime, const std::vector<std::shared_ptr<Monster>> &listeEnemis, std::vector<std::unique_ptr<Projectile>> &listeProjectiles)
{
    // Accumulation du temps pour le cooldown des attaques
    m_tempsDepuisDerniereAtk += deltaTime;

    // Conversion de la portée (cases) en pixels (1 case = 16 pixels)
    float porteeEnPixels = m_portee * 16.0f;
    // Optimisation : calcul au carré pour éviter l'utilisation de std::sqrt gourmande en ressources
    float porteeEnPixelsSq = std::pow(porteeEnPixels, 2);

    // 1. VÉRIFICATION DE LA CIBLE ACTUELLE
    if (m_cible) {
        if (m_cible->isDead()) { 
            m_cible = nullptr; // Oubli du monstre s'il est mort
        } else {
            sf::Vector2f posCible = m_cible->getPosition();
            float distSq = std::pow(posCible.x - m_position.x, 2) + std::pow(posCible.y - m_position.y, 2);
            if (distSq > porteeEnPixelsSq) {
                m_cible = nullptr; // La cible a dépassé le rayon d'action de la tour
            }
        }
    }

    // 2. RECHERCHE AUTOMATIQUE D'UNE NOUVELLE CIBLE (Si libre)
    if (!m_cible) {
        float distanceMinSq = porteeEnPixelsSq; 
        for (const auto &enemi : listeEnemis) {
            if (enemi && !enemi->isDead()) {
                sf::Vector2f posEnemi = enemi->getPosition();
                float distSq = std::pow(posEnemi.x - m_position.x, 2) + std::pow(posEnemi.y - m_position.y, 2);
                
                // On sélectionne le monstre le plus proche de la tour
                if (distSq <= distanceMinSq) { 
                    m_cible = enemi; 
                    distanceMinSq = distSq; 
                }
            }
        }
    }

    // 3. DÉCLENCHEMENT DE L'ATTAQUE (Si le cooldown est prêt)
    if (m_cible && m_tempsDepuisDerniereAtk >= m_vitesseAtk) {
        attaquer(m_cible, listeProjectiles);
        m_tempsDepuisDerniereAtk = 0.0f; // Réinitialisation du chrono de recharge
    }
}

void Tour::attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>>& listeProjectiles)
{
    if (cible) {
        // Logique pure : création du projectile physique dans l'espace mathématique
        listeProjectiles.push_back(std::make_unique<Projectile>(m_position, cible, m_atk, m_type, m_niveau));
        
        // Signal envoyé à la vue : Joue l'effet sonore du tir de manière asynchrone
        m_view->jouerSonTir(); 
    }
}

void Tour::upgrade() {
    if (m_niveau < 3) { m_niveau++; m_atk += 15; }
}

void Tour::tenterAmelioration(int& orJoueur) {
    int coutUpgrade = m_niveau * 50; 
    if (orJoueur >= coutUpgrade) {
        orJoueur -= coutUpgrade; // Déduction de la monnaie par référence
        m_niveau++;              
        m_atk = static_cast<int>(m_atk * 1.30f); // +30% d'efficacité offensive        
        m_portee += 25;                          // Extension de zone       
        m_vitesseAtk *= 0.85f;                   // Réduction du cooldown = cadence augmentée
    }
}
