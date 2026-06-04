#include "../include/Tour.hpp"
#include "mob/Monster.hpp"
#include "Projectile.hpp"
#include <cmath>
#include <iostream>

// ✅ Constructeur corrigé : ajout du paramètre "int modeTir" et correction de l'initialisation de m_modeTir
Tour::Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, int modeTir, sf::Vector2f pos, const std::string &texturePath)
    : m_id(id), m_atk(atk), m_valeur(valeur), m_type(type), m_portee(portee),
      m_vitesseAtk(vitesseAtk), m_modeTir(modeTir), m_position(pos), m_valeurEvo(valeur * 0.6f),
      m_tempsDepuisDerniereAtk(0.0f),
      m_sprite(m_texture) 
{
    if (m_texture.loadFromFile(texturePath))
    {
        m_sprite.setTexture(m_texture);
        
        auto textureSize = m_texture.getSize();
        m_sprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(textureSize.x), static_cast<int>(textureSize.y)}));

        auto bounds = m_sprite.getLocalBounds();
        m_sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

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

    if (m_cible)
    {
        if (m_cible->isDead())
        {
            m_cible = nullptr; 
        }
        else
        {
            sf::Vector2f posCible = m_cible->getPosition();

            float distSq = std::pow(posCible.x - m_position.x, 2) +
                           std::pow(posCible.y - m_position.y, 2);

            if (distSq > std::pow(m_portee, 2))
            {
                m_cible = nullptr; 
            }
        }
    }

    if (!m_cible)
    {
        float distanceMinSq = std::pow(m_portee, 2);

        for (const auto &enemi : listeEnemis)
        {
            if (enemi && !enemi->isDead())
            {
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

    if (m_cible && m_tempsDepuisDerniereAtk >= m_vitesseAtk)
    {
        attaquer(m_cible, listeProjectiles);
        m_tempsDepuisDerniereAtk = 0.0f; 
    }
}

void Tour::draw(sf::RenderWindow &window) const
{
    window.draw(m_sprite);
}

// ✅ Fonction attaquée corrigée : Traduction du chiffre m_modeTir vers le projectile
void Tour::attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>>& listeProjectiles)
{
    if (cible)
    {
        // On traduit le int (0 ou 1) de la tour vers l'Enum (Espace ou Continu) attendu par le projectile
        ModeTir modeDuLaser = (m_modeTir == 1) ? ModeTir::Continu : ModeTir::Espace;

        // On passe "modeDuLaser" en dernier paramètre !
        listeProjectiles.push_back(std::make_unique<Projectile>(m_position, cible, m_atk, m_type, modeDuLaser));
        
        std::cout << "[TOUR] Tir d'un rayon laser (" << m_type << ") en mode chiffre : " << m_modeTir << " !\n";
    }
}