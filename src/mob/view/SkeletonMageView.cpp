#include "SkeletonMageView.hpp"

// SFML 3 : On lie obligatoirement m_sprite à m_textureDroite dès le départ
SkeletonMageView::SkeletonMageView() : m_sprite(m_textureDroite) {
    
    // 1. Chargement des textures  C:\Users\bilal\OneDrive\Documents\Projet_tower_defence\asset\Skeleton_Crew\Skeleton-Mage\Run
    if (!m_textureDroite.loadFromFile("asset/Skeleton_Crew/Skeleton-Mage/Run/RunMage.png")) {
        std::cerr << "Erreur : Impossible de charger Run-Sheet.png" << std::endl;
    }
    if (!m_textureGauche.loadFromFile("asset/Skeleton_Crew/Skeleton-Mage/Run/RunMage_Reverse.png")) {
        std::cerr << "Erreur : Impossible de charger Run_Reverse.png" << std::endl;
    }

    if (!m_textureMort.loadFromFile("asset/Skeleton_Crew/Skeleton-Mage/Death/Death-Sheet.png")) {
        std::cerr << "Erreur : Impossible de charger Death-Sheet.png" << std::endl;
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

void SkeletonMageView::update(float deltaTime, const Monster& logique) {
    // A. On aligne la position du sprite sur la position mathématique calculée par la logique
    if (m_animationMortTermine) return;

    m_sprite.setPosition(logique.getPosition());

    if (logique.estMort()) {
        // --- 1. CODE DE MORT SÉCURISÉ PAR TABLEAU ---
        
        // On définit manuellement les 8 rectangles exacts des 8 squelettes visibles
        // Si ton image fait 576 pixels de large avec 8 squelettes, chaque case fait EXACTEMENT 72 pixels de large !
        // (72 * 8 = 576). C'est pour ça que le calcul avec 64 pixels décalait tout et clignotait !
        static const sf::IntRect rectanglesMort[] = {
    sf::IntRect({0,   0}, {64, 64}),
    sf::IntRect({64,  0}, {64, 64}),
    sf::IntRect({128, 0}, {64, 64}),
    sf::IntRect({192, 0}, {64, 64}),
    sf::IntRect({256, 0}, {64, 64}),
    sf::IntRect({320, 0}, {64, 64})
};

        // Sécurité : Premier instant de la mort, on initialise
        if (&m_sprite.getTexture() != &m_textureMort) {
        m_sprite.setTexture(m_textureMort);
        m_sprite.setOrigin({ 64.f / 2.f, 52.f }); // ← adapte l'origine à la nouvelle largeur de frame
        m_frameActuelle = 0;
        m_tempsAnimation = 0.f;
        m_sprite.setTextureRect(rectanglesMort[0]);
     }

        // On supprime l'affichage des barres de vie
        m_barrePV.setSize({0.f, 0.f});
        m_barreFond.setSize({0.f, 0.f});

        m_tempsAnimation += deltaTime;
        
        // On ralentit à 0.15s pour bien voir les images s'enchaîner proprement
        if (m_tempsAnimation >= 0.15f) { 
            m_tempsAnimation = 0.f;
            
            // 
            if (m_frameActuelle < 5) { // On a 6 frames (index 0 à 5)
                m_frameActuelle++;
                // On applique le rectangle exact sans faire de calcul mathématique multiplicateur
                m_sprite.setTextureRect(rectanglesMort[m_frameActuelle]);
            } else {
                // On est resté bloqué sur la dernière image (index 7), l'animation est finie
                m_animationMortTermine = true; 
            }
        }
        return; 
    }

    // --- 2. CODE DE COURSE INCHANGÉ (Qui utilise tes cases de 64x64) ---
    sf::Vector2f posMonstre = logique.getPosition();
    m_barreFond.setPosition({posMonstre.x, posMonstre.y - 30.f});
    m_barrePV.setPosition({posMonstre.x, posMonstre.y - 30.f});

    float ratioVie = static_cast<float>(logique.getPvActuels()) / static_cast<float>(logique.getPvMax());
    if (ratioVie < 0.f) ratioVie = 0.f;
    m_barrePV.setSize({40.f * ratioVie, 5.f});

    if (logique.getDirection() == Monster::Direction::Gauche) {
        m_sprite.setTexture(m_textureGauche);
    } else {
        m_sprite.setTexture(m_textureDroite);
    }

    if (!logique.estArrive()) {
        m_tempsAnimation += deltaTime;
        if (m_tempsAnimation >= m_vitesseAnimation) {
            m_tempsAnimation = 0.f;
            m_frameActuelle = (m_frameActuelle + 1) % m_nbFramesMax;

            int positionXDecoupe = m_frameActuelle * m_frameLargeur; // Garde tes 64 pixels ici
            m_sprite.setTextureRect(sf::IntRect(
                {positionXDecoupe, 0}, 
                {m_frameLargeur, m_frameHauteur}
            ));
        }
    }
}
void SkeletonMageView::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
    window.draw(m_barreFond);
    window.draw(m_barrePV);
}