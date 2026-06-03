#include "SkeletonView.hpp"

// SFML 3 : On lie obligatoirement m_sprite à m_textureDroite dès le départ
SkeletonView::SkeletonView() : m_sprite(m_textureDroite) {
    
    // 1. Chargement des textures  C:\Users\bilal\OneDrive\Documents\Projet_tower_defence\asset\Skeleton_Crew\Skeleton-Base\Run
    if (!m_textureDroite.loadFromFile("asset/Skeleton_Crew/Skeleton-Base/Run/Run2.png")) {
        std::cerr << "Erreur : Impossible de charger Run-Sheet.png" << std::endl;
    }
    if (!m_textureGauche.loadFromFile("asset/Skeleton_Crew/Skeleton-Base/Run/Run_Reverse.png")) {
        std::cerr << "Erreur : Impossible de charger Run_Reverse.png" << std::endl;
    }

    // 2. Découpe initiale de la première case (0, 0)
    m_sprite.setTextureRect(sf::IntRect({0, 0}, {m_frameLargeur, m_frameHauteur}));
    
    // 3. On décale le pivot vertical (52.f) pour remonter le corps du squelette sur les pavés
    m_sprite.setOrigin({ static_cast<float>(m_frameLargeur) / 2.f, 52.f });

    m_barreFond.setSize({40.f, 5.f});
    m_barreFond.setFillColor(sf::Color::Red);
    // On centre l'origine de la barre de fond pour la caler facilement au-dessus du monstre
    m_barreFond.setOrigin({20.f, 0.f}); 

    m_barrePV.setSize({40.f, 5.f});
    m_barrePV.setFillColor(sf::Color::Green);
    m_barrePV.setOrigin({20.f, 0.f});
}

void SkeletonView::update(float deltaTime, const Monster& logique) {
    // A. On aligne la position du sprite sur la position mathématique calculée par la logique
    m_sprite.setPosition(logique.getPosition());

    sf::Vector2f posMonstre = logique.getPosition();
    m_barreFond.setPosition({posMonstre.x, posMonstre.y - 30.f});
    m_barrePV.setPosition({posMonstre.x, posMonstre.y - 30.f});

    float ratioVie = logique.getPvActuels() / logique.getPvMax();
    if (ratioVie < 0.f) ratioVie = 0.f;

    m_barrePV.setSize({40.f * ratioVie, 5.f});

    // B. Changement automatique de texture selon la direction de la logique
    if (logique.getDirection() == Monster::Direction::Gauche) {
        m_sprite.setTexture(m_textureGauche);
    } else {
        m_sprite.setTexture(m_textureDroite);
    }

    // C. Logique de l'animation par décalage de frame (uniquement s'il n'est pas arrivé)
    if (!logique.estArrive()) {
        m_tempsAnimation += deltaTime;
        
        if (m_tempsAnimation >= m_vitesseAnimation) {
            m_tempsAnimation = 0.f;
            m_frameActuelle = (m_frameActuelle + 1) % m_nbFramesMax; // Boucle de 0 à 5

            int positionXDecoupe = m_frameActuelle * m_frameLargeur;
            
            // On applique la nouvelle zone de découpe SFML 3
            m_sprite.setTextureRect(sf::IntRect(
                {positionXDecoupe, 0}, 
                {m_frameLargeur, m_frameHauteur}
            ));
        }
    }
}

void SkeletonView::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
    window.draw(m_barreFond);
    window.draw(m_barrePV);
}