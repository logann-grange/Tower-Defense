#include "../include/Tour.hpp"
#include "mob/Monster.hpp"
#include "Projectile.hpp"
#include <cmath>
#include <iostream>
#include "SFML/Audio.hpp"

Tour::Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, int niveau, sf::Vector2f pos, const std::string &texturePath)
    : m_id(id), m_atk(atk), m_valeur(valeur), m_type(type), m_portee(portee),
      m_vitesseAtk(vitesseAtk), m_niveau(niveau), m_position(pos), m_valeurEvo(valeur * 0.6f),
      m_tempsDepuisDerniereAtk(0.0f),
      m_sprite(m_texture), // Correction SFML 3 obligatoire : Lier le sprite à sa texture dans la liste d'initialisation
      m_sound(m_soundBuffer)    // On lie le Sound à son SoundBuffer dès le départ
{
    if (m_texture.loadFromFile(texturePath))
    {
        // Le paramètre 'true' force SFML 3 à réinitialiser proprement la taille géométrique interne du Sprite
        m_sprite.setTexture(m_texture, true); 
        
        auto textureSize = m_texture.getSize();
        m_sprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(textureSize.x), static_cast<int>(textureSize.y)}));

        // Centrage du point de pivot (Origin) au milieu du sprite de la tour
        auto bounds = m_sprite.getLocalBounds();
        m_sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});

        // Ajustement d'échelle général pour les dimensions de la carte
        m_sprite.setScale({0.08f, 0.08f});
        m_sprite.setPosition(m_position);
    }
    else
    {
        std::cerr << "[ERREUR SFML] Impossible de charger la texture de la tour : " << texturePath << "\n";
    }
    // --- CONFIGURATION AUDIO ---
    // Sélection dynamique du fichier audio en fonction de l'élément de la tour
    std::string soundPath = "asset/sound/tir_defaut.wav"; // Chemin de secours
    
    if (m_type == "Feu") {
        soundPath = "asset/sound/tir_feu.wav";
    } else if (m_type == "Glace") {
        soundPath = "asset/sound/tir_glace.wav";
    }

    // Chargement du fichier audio dans le buffer de mémoire
    if (m_soundBuffer.loadFromFile(soundPath)) {
        // Association obligatoire du Sound avec son Buffer de données
        m_sound.setBuffer(m_soundBuffer);
        m_sound.setVolume(50.f); // Réglage du volume initial (0.f à 100.f)
    } else {
        std::cerr << "[WARNING IA AUDIO] Impossible de charger le fichier audio : " << soundPath << "\n";
    }
}

void Tour::update(float deltaTime, const std::vector<std::shared_ptr<Monster>> &listeEnemis, std::vector<std::unique_ptr<Projectile>> &listeProjectiles)
{
    // Incrémentation du compteur de temps de tir
    m_tempsDepuisDerniereAtk += deltaTime;

    // IA LOGIQUE : Conversion de la portée théorique en pixels réels (1 case de grille = 16 pixels)
    float porteeEnPixels = m_portee * 16.0f;
    float porteeEnPixelsSq = std::pow(porteeEnPixels, 2); // Distance au carré pour éviter un std::sqrt coûteux en performance

    // --- ÉTAPE 1 : VÉRIFICATION DE LA CIBLE ACTUELLE ---
    if (m_cible)
    {
        // Si le monstre ciblé est mort, on lâche la cible
        if (m_cible->isDead())
        {
            m_cible = nullptr; 
        }
        else
        {
            sf::Vector2f posCible = m_cible->getPosition();
            float distSq = std::pow(posCible.x - m_position.x, 2) + std::pow(posCible.y - m_position.y, 2);

            // Si le monstre sort du rayon d'action de la tour, on perd le ciblage
            if (distSq > porteeEnPixelsSq)
            {
                m_cible = nullptr; 
            }
        }
    }

    // --- ÉTAPE 2 : RECHERCHE D'UNE NOUVELLE CIBLE (Si aucune active) ---
    // Algorithme de ciblage : Sélectionne le monstre le plus proche du centre de la tour
    if (!m_cible)
    {
        float distanceMinSq = porteeEnPixelsSq; // On cherche uniquement à l'intérieur de notre rayon d'action

        for (const auto &enemi : listeEnemis)
        {
            if (enemi && !enemi->isDead())
            {
                sf::Vector2f posEnemi = enemi->getPosition();
                float distSq = std::pow(posEnemi.x - m_position.x, 2) + std::pow(posEnemi.y - m_position.y, 2);

                // Si ce monstre est plus proche que le précédent retenu, il devient la cible prioritaire
                if (distSq <= distanceMinSq)
                {
                    m_cible = enemi;
                    distanceMinSq = distSq; 
                }
            }
        }
    }

    // --- ÉTAPE 3 : LOGIQUE DE TIR ---
    // Si une cible est acquise et que le cooldown de tir est rechargé
    if (m_cible && m_tempsDepuisDerniereAtk >= m_vitesseAtk)
    {
        attaquer(m_cible, listeProjectiles);
        m_tempsDepuisDerniereAtk = 0.0f; // Réinitialisation du cooldown
    }
}

void Tour::draw(sf::RenderWindow &window) const
{
    window.draw(m_sprite);
}


void Tour::attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>>& listeProjectiles)
{
    if (cible)
    {
        // IA LOGIQUE : Création d'une instance de Projectile en lui passant le niveau actuel de la tour (m_niveau).
        // Cela permet au projectile de charger dynamiquement la bonne texture évolutive (Feu 1, 2, ou 3).
        listeProjectiles.push_back(std::make_unique<Projectile>(m_position, cible, m_atk, m_type, m_niveau));
        // 🔥 ACTION AUDIO : Déclenche la lecture du son de tir de manière asynchrone (ne bloque pas le jeu)
        m_sound.play();
        std::cout << "[TOUR] Tir d'un projectile (" << m_type << ") de niveau " << m_niveau << " !\n";
    }
}

void Tour::upgrade()
{
    if (m_niveau < 3)
    {
        m_niveau++;
        m_atk += 15; 
        std::cout << "[EVOLUTION] La tour " << m_type << " passe au niveau " << m_niveau << " !\n";
    }
}

std::string Tour::getType() const 
{ 
    return m_type; 
}

void Tour::tenterAmelioration(int& orJoueur) {
    // Calcul dynamique du coût en fonction du niveau de la tour (Niveau 1 -> 50G, Niveau 2 -> 100G)
    int coutUpgrade = m_niveau * 50; 

    if (orJoueur >= coutUpgrade) {
        orJoueur -= coutUpgrade; // Déduction directe sur l'or global du joueur passé par référence
        m_niveau++;              // Passage au niveau supérieur
        
        // --- ÉVOLUTION DES STATISTIQUES ---
        m_atk = static_cast<int>(m_atk * 1.30f);        // +30% de dégâts bruts (m_atk)
        m_portee += 25;                                 // +25 unités de portée sur la carte
        m_vitesseAtk *= 0.85f;                          // Réduction du cooldown de 15% (la tour tire plus vite)

        std::cout << "SUCCESS : Tour amelioree au Niveau " << m_niveau 
                  << " (Cout : " << coutUpgrade << " Or) !" << std::endl;
    } else {
        std::cout << "Pas assez d'or ! Il faut " << coutUpgrade << " Or pour ameliorer." << std::endl;
    }
}