#include "../include/Projectile.hpp"
#include "mob/Monster.hpp" 
#include <cmath>
#include <iostream>

// 💡 SOLUTION SFML 3 : Texture par défaut obligatoire pour le sprite
static sf::Texture textureParDefaut;

Projectile::Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string &typeTour, int niveauTour)
    : m_cible(cible), m_positionActuelle(positionDepart), m_degats(degats),
      m_typeTour(typeTour), m_niveau(niveauTour), m_detruit(false),
      m_sprite(textureParDefaut) 
{
    // Décalage vertical pour faire sortir le projectile du haut de la tourelle
    m_positionActuelle.y -= 16.f; 

    // 💡 GESTION DU STOCKAGE DES TEXTURES (Feu + Glace)
    static sf::Texture texFeu1, texFeu2, texFeu3;
    static sf::Texture texGlace1, texGlace2, texGlace3;
    static bool initialise = false;

    // Chargement unique de toutes les images du jeu
    if (!initialise) {
        // --- TEXTURES DE FEU ---
        bool f1 = texFeu1.loadFromFile("asset/projectile_feu/projectile_feu_1.png");
        bool f2 = texFeu2.loadFromFile("asset/projectile_feu/projectile_feu_4.png");
        bool f3 = texFeu3.loadFromFile("asset/projectile_feu/projectile_feu_6.png");
        
        // --- TEXTURES DE GLACE --- (Assure-toi que les noms de fichiers correspondent sur ton disque)
        bool g1 = texGlace1.loadFromFile("asset/projectile_glace/projectile_glace_1.png");
        bool g2 = texGlace2.loadFromFile("asset/projectile_glace/projectile_glace_2.png");
        bool g3 = texGlace3.loadFromFile("asset/projectile_glace/projectile_glace_3.png");

        if (!f1 || !f2 || !f3 || !g1 || !g2 || !g3) {
            std::cerr << "[ERREUR CRITIQUE] Impossible de charger un ou plusieurs assets de projectiles !\n";
        }
        initialise = true;
    }

    float echelle = 0.35f; 

    // 💡 ATTRIBUTION DE LA TEXTURE ET DE L'ÉCHELLE EN FONCTION DU TYPE ET DU NIVEAU
    if (m_typeTour == "Feu") {
        if (m_niveau == 2) {
            m_sprite.setTexture(texFeu2, true);
            echelle = 0.45f;
        } 
        else if (m_niveau >= 3) {
            m_sprite.setTexture(texFeu3, true);
            echelle = 0.30f; 
        }
        else {
            m_sprite.setTexture(texFeu1, true);
            echelle = 0.35f;
        }
    } 
    else if (m_typeTour == "Glace") {
        if (m_niveau == 2) {
            m_sprite.setTexture(texGlace2, true);
            echelle = 0.18f; // Échelle adaptée car les cristaux sont naturellement plus larges
        } 
        else if (m_niveau >= 3) {
            m_sprite.setTexture(texGlace3, true);
            echelle = 0.22f; // Gros bloc de cristal final
        }
        else {
            m_sprite.setTexture(texGlace1, true);
            echelle = 0.25f; // Petite comète de glace
        }
    }
    else {
        m_sprite.setTexture(texFeu1, true); // Sécurité par défaut
    }

    // Centrage automatique du pivot sur l'image active
    auto size = m_sprite.getTexture().getSize(); 
    m_sprite.setOrigin({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f});
    
    m_sprite.setScale({echelle, echelle}); 
    m_sprite.setPosition(m_positionActuelle);
}

void Projectile::update(float deltaTime) {
    if (m_detruit) return;

    if (!m_cible || m_cible->isDead()) {
        m_detruit = true;
        return;
    }

    sf::Vector2f posCible = m_cible->getPosition();
    posCible.y -= 24.f; // Vise le torse du squelette

    sf::Vector2f direction = posCible - m_positionActuelle;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    if (distance <= 10.f) {
        m_cible->takeDamage(m_degats); 
        m_detruit = true;             
        return;
    }

    sf::Vector2f directionNormalisee = direction / distance;
    m_positionActuelle += directionNormalisee * m_vitesse * deltaTime;
    
    m_sprite.setPosition(m_positionActuelle);

    // Orientation automatique du sprite vers le squelette
    float const PI = 3.14159265f;
    float angle = std::atan2(direction.y, direction.x) * 180.f / PI;
    m_sprite.setRotation(sf::degrees(angle)); 
}

void Projectile::draw(sf::RenderWindow &window) const {
    if (!m_detruit) {
        window.draw(m_sprite);
    }
}