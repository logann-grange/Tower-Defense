#include "../include/Tour.hpp"
#include "mob/Monster.hpp"
#include "Projectile.hpp"
#include <cmath>
#include <iostream>

// ✅ Constructeur SFML 3 corrigé : Utilisation stricte de m_niveau au lieu de modeTir
Tour::Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, int niveau, sf::Vector2f pos, const std::string &texturePath)
    : m_id(id), m_atk(atk), m_valeur(valeur), m_type(type), m_portee(portee),
      m_vitesseAtk(vitesseAtk), m_niveau(niveau), m_position(pos), m_valeurEvo(valeur * 0.6f),
      m_tempsDepuisDerniereAtk(0.0f),
      m_sprite(m_texture) // ✅ Initialisation requise en SFML 3
{
    if (m_texture.loadFromFile(texturePath))
    {
        // ✅ Le paramètre 'true' force SFML 3 à recalculer la taille interne du Sprite
        m_sprite.setTexture(m_texture, true); 
        
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

    // 💡 Astuce : On convertit la portée (ex: 3 cases) en pixels (3 * 16 = 48 pixels)
    float porteeEnPixels = m_portee * 16.0f;
    float porteeEnPixelsSq = std::pow(porteeEnPixels, 2);

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

            // ✅ Utilisation de la portée corrigée en pixels
            if (distSq > porteeEnPixelsSq)
            {
                m_cible = nullptr; 
            }
        }
    }

    if (!m_cible)
    {
        // ✅ On commence la recherche avec notre rayon d'action maximum en pixels
        float distanceMinSq = porteeEnPixelsSq;

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

// ✅ Fonction d'attaque : Transmet le niveau de manière 100% fiable au projectile
void Tour::attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>>& listeProjectiles)
{
    if (cible)
    {
        // On donne m_niveau pour charger l'image correspondante (1, 2 ou 3) !
        listeProjectiles.push_back(std::make_unique<Projectile>(m_position, cible, m_atk, m_type, m_niveau));
        
        std::cout << "[TOUR] Tir d'un projectile (" << m_type << ") de niveau " << m_niveau << " !\n";
    }
}

// 💡 BONUS : Ajout de la fonction d'amélioration pour tester tes projectiles géants en jeu !
void Tour::upgrade()
{
    if (m_niveau < 3)
    {
        m_niveau++;
        m_atk += 15; // Augmente la puissance
        std::cout << "[EVOLUTION] La tour " << m_type << " passe au niveau " << m_niveau << " !\n";
    }
}

std::string Tour::getType() const 
{ 
    return m_type; 
}

void Tour::tenterAmelioration(int& orJoueur) {
    // Le coût augmente à chaque niveau (Niveau 1 -> 50 Or, Niveau 2 -> 100 Or...)
    int coutUpgrade = m_niveau * 50; 

    if (orJoueur >= coutUpgrade) {
        orJoueur -= coutUpgrade; // On déduit l'or du joueur
        m_niveau++;              // On monte de niveau
        
        // --- BONUS DE STATS CORRIGÉ AVEC TES VARIABLES ---
        m_atk = static_cast<int>(m_atk * 1.30f);        // +30% d'attaque (m_atk)
        m_portee += 25;                                 // +25 de portée (m_portee est un int chez toi)
        m_vitesseAtk *= 0.85f;                          // Réduit le temps d'attente de 15% -> Tire plus vite !

        std::cout << "SUCCESS : Tour amelioree au Niveau " << m_niveau 
                  << " (Cout : " << coutUpgrade << " Or) !" << std::endl;
    } else {
        std::cout << "Pas assez d'or ! Il faut " << coutUpgrade << " Or pour ameliorer." << std::endl;
    }
}
