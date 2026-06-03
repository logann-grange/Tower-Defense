#include "Skeleton.hpp"
#include <iostream>

Skeleton::Skeleton()
    : m_texture(), m_sprite(m_texture)
{
    maxHealth = health = 50;
    speed = 30.0f;
    damage = 5;
    reward = 10;
    x = 0;
    y = 0;
    dmg = 5;
    faiblesse = "Fire";
    lienSprit = "asset/Skeleton_Crew/Skeleton-Base/Run/Run2.png";
    m_indexEtapeActuelle = 0;


if (!m_texture.loadFromFile(lienSprit)) { // Mets le bon chemin vers ton PNG
        std::cerr << "Erreur : Impossible de charger " << lienSprit << std::endl;
    }
    m_sprite.setTexture(m_texture);

    // 2. Découper uniquement le premier carré (Frame 0)
    // sf::IntRect({position_x, position_y}, {largeur, hauteur})
    m_sprite.setTextureRect(sf::IntRect({0, 0}, {m_frameLargeur, m_frameHauteur}));

    // 3. Optionnel : Centrer l'origine du sprite pour qu'il soit bien aligné sur tes cases
    m_sprite.setOrigin({static_cast<float>(m_frameLargeur) / 2.f,52.f});
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

    // --- 2. GESTION DE L'ANIMATION ---
    m_tempsAnimation += deltaTime;

    if (m_tempsAnimation >= m_vitesseAnimation) {
        m_tempsAnimation = 0.f; // On réinitialise le compteur de temps
        m_frameActuelle++;      // On passe à l'image suivante

        // Si on dépasse la 8ème image, on revient au tout début en boucle
        if (m_frameActuelle >= m_nbFramesMax) {
            m_frameActuelle = 0;
        }

        // CALCUL DE LA ZONE À DÉCOUPER
        // La position X de la découpe se décale de 64 pixels à chaque frame (0, 64, 128, 192...)
        int positionXDecoupe = m_frameActuelle * m_frameLargeur;

        // On applique le nouveau rectangle de découpe SFML 3
        m_sprite.setTextureRect(sf::IntRect(
            {positionXDecoupe, 0}, 
            {m_frameLargeur, m_frameHauteur}
        ));
}
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
