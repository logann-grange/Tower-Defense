#include "Skeleton.hpp"
#include <iostream>

Skeleton::Skeleton()
    : m_texture(), m_sprite(m_texture)
{
    maxHealth = health = 50;
    speed = 1.0f;
    damage = 5;
    reward = 10;
    x = 0;
    y = 0;
    dmg = 5;
    faiblesse = "Fire";
    lienSprit = "assets/Skeleton_Crew/Skeleton - Base/Run/Run-Sheet.png";
    m_indexEtapeActuelle = 0;

    if (!m_texture.loadFromFile(lienSprit)) {
        std::cerr << "Erreur : Impossible de charger le sprite du squelette\n";
    }
    m_sprite.setTexture(m_texture);
    
    // Si votre Run-Sheet contient plusieurs images alignées (spritesheet),
    // on découpe le premier squelette pour l'instant (ex: un carré de 32x32 ou 16x16)
    m_sprite.setTextureRect(sf::IntRect({0, 0}, {32, 32}));
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
    // Si le squelette a atteint la fin du chemin
    if (m_indexEtapeActuelle >= m_chemin.size()) {
        attaquer();
        // Ici, vous pourrez infliger des dégâts au joueur et détruire le monstre
        return;
    }

    // Cible actuelle en pixels (on vise le centre ou le coin de la case, ici le coin)
    sf::Vector2f ciblePixels(
        static_cast<float>(m_chemin[m_indexEtapeActuelle].x * 16),
        static_cast<float>(m_chemin[m_indexEtapeActuelle].y * 16)
    );

    // Calcul du vecteur direction vers la cible
    sf::Vector2f direction = ciblePixels - m_positionPixels;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // Distance que le squelette peut parcourir pendant cette image (ce frame)
    float distanceDeplacement = speed * deltaTime;

    if (distance <= distanceDeplacement) {
        // Le squelette est arrivé sur la case ciblée, on le place pile dessus
        m_positionPixels = ciblePixels;
        m_indexEtapeActuelle++; // On vise la case suivante au prochain tour !
    } else {
        // On normalise le vecteur direction et on avance
        sf::Vector2f directionNormalisee = direction / distance;
        m_positionPixels += directionNormalisee * distanceDeplacement;
    }

    // Appliquer la nouvelle position au sprite SFML
    m_sprite.setPosition(m_positionPixels);
}

void Skeleton::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
}


void Skeleton::mourir() {
    std::cout << "Skeleton meurt et récompense le joueur de " << reward << " pièces\n";
    
}

void Skeleton::attaquer() {
    std::cout << "Skeleton attaque et inflige " << damage << " dégâts\n";
}
