#include "Skeleton.hpp"
#include <iostream>

Skeleton::Skeleton()
    : Monster() // ✅ Appel explicite du constructeur de Monster
{
    maxHealth = health = 50;
    speed = 80.0f;
    damage = 5;
    reward = 10;
    x = 0;
    y = 0;
    dmg = 5;
    faiblesse = "Fire";
    lienSprit = "asset/Skeleton_Crew/Skeleton-Base/Run/Run2.png";
    m_indexEtapeActuelle = 0;

    if (!m_texture.loadFromFile(lienSprit)) { 
        std::cerr << "Erreur : Impossible de charger " << lienSprit << std::endl;
    }
    
    // ✅ On applique la texture sur m_sprite (hérité de Monster)
    m_sprite.setTexture(m_texture);

    // Découper uniquement le premier carré (Frame 0)
    m_sprite.setTextureRect(sf::IntRect({0, 0}, {m_frameLargeur, m_frameHauteur}));

    // Centrer l'origine du sprite
    m_sprite.setOrigin({static_cast<float>(m_frameLargeur) / 2.f, 52.f});
}

void Skeleton::spawn(const std::vector<sf::Vector2i>& cheminPoints) {
    if (cheminPoints.empty()) return;
    m_chemin = cheminPoints;
    m_indexEtapeActuelle = 1;

    m_positionPixels.x = static_cast<float>(m_chemin[0].x * 16);
    m_positionPixels.y = static_cast<float>(m_chemin[0].y * 16);
    m_sprite.setPosition(m_positionPixels);
}

void Skeleton::move(float deltaTime) {
    // 💀 1. GESTION DE LA MORT : Met à jour l'effet visuel de dissolution/clignotement
    updateDeathAnimation(deltaTime);

    // 🛑 2. SI LE SQUELETTE EST EN TRAIN DE MOURIR, ON S'ARRÊTE ICI !
    if (m_isDying) {
        return; // Coupe les mouvements, les animations de course et le suivi de chemin
    }

    // Si le squelette a atteint la fin du chemin
    if (m_indexEtapeActuelle >= m_chemin.size()) {
        attaquer();
        return;
    }

    // Cible actuelle en pixels
    sf::Vector2f ciblePixels(
        static_cast<float>(m_chemin[m_indexEtapeActuelle].x * 16),
        static_cast<float>(m_chemin[m_indexEtapeActuelle].y * 16)
    );

    // Calcul du vecteur direction vers la cible
    sf::Vector2f direction = ciblePixels - m_positionPixels;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Distance que le squelette peut parcourir pendant cette image
    float distanceDeplacement = speed * deltaTime;

    if (distance <= distanceDeplacement) {
        m_positionPixels = ciblePixels;
        m_indexEtapeActuelle++; 
    } else {
        sf::Vector2f directionNormalisee = direction / distance;
        m_positionPixels += directionNormalisee * distanceDeplacement;
    }

    // Appliquer la nouvelle position au sprite SFML
    m_sprite.setPosition(m_positionPixels);
    
    // --- GESTION DE L'ANIMATION DE COURSE ---
    m_tempsAnimation += deltaTime;

    if (m_tempsAnimation >= m_vitesseAnimation) {
        m_tempsAnimation = 0.f; 
        m_frameActuelle++;      

        if (m_frameActuelle >= m_nbFramesMax) {
            m_frameActuelle = 0;
        }

        int positionXDecoupe = m_frameActuelle * m_frameLargeur;

        m_sprite.setTextureRect(sf::IntRect(
            {positionXDecoupe, 0}, 
            {m_frameLargeur, m_frameHauteur}
        ));
    }
}

void Skeleton::draw(sf::RenderWindow& window) {
    // ✅ Dessine le sprite hérité (qui clignote et s'efface !)
    window.draw(m_sprite);
}

void Skeleton::mourir() {
    std::cout << "Skeleton meurt et récompense le joueur de " << reward << " pièces\n";
}

void Skeleton::attaquer() {
    std::cout << "Skeleton attaque et inflige " << damage << " dégâts\n";
}