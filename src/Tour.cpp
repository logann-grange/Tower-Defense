#include "../include/Tour.hpp"
#include "mob/Monster.hpp"
#include "Projectile.hpp"
#include <cmath>
#include <iostream>

Tour::Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, sf::Vector2f pos, const std::string &texturePath)
    : m_id(id), m_atk(atk), m_valeur(valeur), m_type(type), m_portee(portee),
      m_vitesseAtk(vitesseAtk), m_position(pos), m_valeurEvo(valeur * 0.6f),
      m_tempsDepuisDerniereAtk(0.0f),
      m_sprite(m_texture) // ✅ CORRECTION SFML 3 : On lie la texture immédiatement ici !
{
    if (m_texture.loadFromFile(texturePath))
    {
        // On rafraîchit le sprite maintenant que la texture est chargée en mémoire
        m_sprite.setTexture(m_texture);
        
        auto textureSize = m_texture.getSize();
        m_sprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(textureSize.x), static_cast<int>(textureSize.y)}));

        auto bounds = m_sprite.getLocalBounds();
        m_sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

        // Ajustement de l'échelle à 12% de la taille d'origine
        m_sprite.setScale({0.08f, 0.08f});
        m_sprite.setPosition(m_position);
    }
    else
    {
        std::cout << "[ERREUR SFML] Impossible de charger la texture de la tour : " << texturePath << "\n";
    }
}

void Tour::update(float deltaTime, const std::vector<std::shared_ptr<Monster>> &listeEnemis, std::vector<std::unique_ptr<Projectile>> &listeProjectiles)
{
    m_tempsDepuisDerniereAtk += deltaTime;

    // 1. Vérification de la cible actuelle
    if (m_cible)
    {
        if (m_cible->isDead())
        {
            m_cible = nullptr; 
        }
        else
        {
            // ✅ CORRECTION : Utilisation des vrais pixels du monstre
            sf::Vector2f posCible = m_cible->getPosition();

            float distSq = std::pow(posCible.x - m_position.x, 2) +
                           std::pow(posCible.y - m_position.y, 2);

            // Si elle sort de la portée réelle
            if (distSq > std::pow(m_portee, 2))
            {
                m_cible = nullptr; 
            }
        }
    }

    // 2. Recherche d'une cible si la tour n'en a pas
    if (!m_cible)
    {
        float distanceMinSq = std::pow(m_portee, 2);

        for (const auto &enemi : listeEnemis)
        {
            if (enemi && !enemi->isDead())
            {
                // ✅ CORRECTION : Utilisation des vrais pixels du monstre
                sf::Vector2f posEnemi = enemi->getPosition();

                float distSq = std::pow(posEnemi.x - m_position.x, 2) +
                               std::pow(posEnemi.y - m_position.y, 2);

                if (distSq <= distanceMinSq)
                {
                    m_cible = enemi;
                    distanceMinSq = distSq; 
                }
            }
        }
    }

    // 3. Condition d'attaque : si on a une cible et que la cadence le permet
    if (m_cible && m_tempsDepuisDerniereAtk >= m_vitesseAtk)
    {
        attaquer(m_cible, listeProjectiles);
        m_tempsDepuisDerniereAtk = 0.0f; // Réinitialise le chrono à zéro
    }
}

void Tour::draw(sf::RenderWindow &window) const
{
    // ✅ CORRECTION SFML 3 : On dessine directement la sprite
    window.draw(m_sprite);
}
void Tour::attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>>& listeProjectiles)
{
    if (cible)
    {
        // On envoie la position de base de la tour, le projectile s'occupera du sommet !
        listeProjectiles.push_back(std::make_unique<Projectile>(m_position, cible, m_atk, m_type));
        std::cout << "[TOUR] Tir d'un rayon laser !\n";
    }
}