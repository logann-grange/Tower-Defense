#include "../include/Tour.hpp"
#include <cmath>
#include <iostream>

Tour::Tour() : m_id(0), m_atk(0), m_valeur(0), m_type(""), m_bonus(""),
               m_valeurEvo(0), m_portee(0), m_vitesseAtk(0.f), m_niveau(0) {}


Tour::Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, int niveau, sf::Vector2f pos, const std::string &texturePath)
    : m_id(id), m_atk(atk), m_valeur(valeur), m_type(type), m_portee(portee),
      m_vitesseAtk(vitesseAtk), m_niveau(niveau), m_position(pos), m_valeurEvo(valeur * 0.6f),
      m_tempsDepuisDerniereAtk(0.0f)
{
    if (m_texture.loadFromFile(texturePath))
    {
        m_sprite.emplace(m_texture);
        
        auto textureSize = m_texture.getSize();
        m_sprite->setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(textureSize.x), static_cast<int>(textureSize.y)}));

        auto bounds = m_sprite->getLocalBounds();
        m_sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        m_sprite->setScale({0.08f, 0.08f});
        m_sprite->setPosition(m_position);
    }
    else
    {
        std::cout << "[ERREUR] Impossible de charger : " << texturePath << "\n";
    }
}

Tour::Tour(const Tour& other)
    : m_id(other.m_id), m_atk(other.m_atk), m_valeur(other.m_valeur),
      m_type(other.m_type), m_bonus(other.m_bonus), m_valeurEvo(other.m_valeurEvo),
      m_position(other.m_position), m_texture(other.m_texture),
      m_portee(other.m_portee), m_vitesseAtk(other.m_vitesseAtk),
      m_tempsDepuisDerniereAtk(other.m_tempsDepuisDerniereAtk), m_niveau(other.m_niveau)
{
    if (other.m_sprite.has_value()) {
        m_sprite.emplace(m_texture);  // rebind sur la nouvelle texture
        // recopie toutes les transformations
        m_sprite->setTextureRect(other.m_sprite->getTextureRect());
        m_sprite->setOrigin(other.m_sprite->getOrigin());
        m_sprite->setScale(other.m_sprite->getScale());
        m_sprite->setPosition(other.m_sprite->getPosition());
    }
}

Tour& Tour::operator=(const Tour& other) {
    if (this == &other) return *this;
    m_id = other.m_id;
    m_atk = other.m_atk;
    m_valeur = other.m_valeur;
    m_type = other.m_type;
    m_bonus = other.m_bonus;
    m_valeurEvo = other.m_valeurEvo;
    m_position = other.m_position;
    m_texture = other.m_texture;
    m_portee = other.m_portee;
    m_vitesseAtk = other.m_vitesseAtk;
    m_tempsDepuisDerniereAtk = other.m_tempsDepuisDerniereAtk;
    m_niveau = other.m_niveau;

    if (other.m_sprite.has_value()) {
        m_sprite.emplace(m_texture);
        m_sprite->setTextureRect(other.m_sprite->getTextureRect());
        m_sprite->setOrigin(other.m_sprite->getOrigin());
        m_sprite->setScale(other.m_sprite->getScale());
        m_sprite->setPosition(other.m_sprite->getPosition());
    }
    return *this;
}

// void Tour::update(float deltaTime, const std::vector<std::shared_ptr<Monster>> &listeEnemis, std::vector<std::unique_ptr<Projectile>> &listeProjectiles)
// {
//     m_tempsDepuisDerniereAtk += deltaTime;

//     float porteeEnPixels = m_portee * 16.0f;
//     float porteeEnPixelsSq = std::pow(porteeEnPixels, 2);

//     if (m_cible)
//     {
//         if (m_cible->isDead())
//         {
//             m_cible = nullptr; 
//         }
//         else
//         {
//             sf::Vector2f posCible = m_cible->getPosition();

//             float distSq = std::pow(posCible.x - m_position.x, 2) +
//                            std::pow(posCible.y - m_position.y, 2);

//             if (distSq > porteeEnPixelsSq)
//             {
//                 m_cible = nullptr; 
//             }
//         }
//     }

//     if (!m_cible)
//     {
//         float distanceMinSq = porteeEnPixelsSq;

//         for (const auto &enemi : listeEnemis)
//         {
//             if (enemi && !enemi->isDead())
//             {
//                 sf::Vector2f posEnemi = enemi->getPosition();

//                 float distSq = std::pow(posEnemi.x - m_position.x, 2) +
//                                std::pow(posEnemi.y - m_position.y, 2);

//                 if (distSq <= distanceMinSq)
//                 {
//                     m_cible = enemi;
//                     distanceMinSq = distSq; 
//                 }
//             }
//         }
//     }

//     if (m_cible && m_tempsDepuisDerniereAtk >= m_vitesseAtk)
//     {
//         attaquer(m_cible, listeProjectiles);
//         m_tempsDepuisDerniereAtk = 0.0f; 
//     }
// }

void Tour::draw(sf::RenderWindow &window) const
{
    if (m_sprite.has_value())
        window.draw(*m_sprite);
}

// void Tour::attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>>& listeProjectiles)
// {
//     if (cible)
//     {
//         // On donne m_niveau pour charger l'image correspondante (1, 2 ou 3) !
//         listeProjectiles.push_back(std::make_unique<Projectile>(m_position, cible, m_atk, m_type, m_niveau));
        
//         std::cout << "[TOUR] Tir d'un projectile (" << m_type << ") de niveau " << m_niveau << " !\n";
//     }
// }

void Tour::upgrade()
{
    if (m_niveau < 3)
    {
        m_niveau++;
        m_atk += 15; // Augmente la puissance
        std::cout << "[EVOLUTION] La tour " << m_type << " passe au niveau " << m_niveau << " !\n";
    }
}

// std::string Tour::getType() const 
// { 
//     return m_type;
// }
