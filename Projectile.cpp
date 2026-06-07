#include "../include/Projectile.hpp"
#include "mob/Monster.hpp" 
#include <cmath>
#include <iostream>

// IA CONSEIL : SFML 3 exige qu'un Sprite soit lié à une texture valide dès son instanciation.
// Cette texture par défaut statique évite les crashs lors de l'appel du constructeur dans la liste d'initialisation.
static sf::Texture textureParDefaut;

Projectile::Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string &typeTour, int niveauTour)
    : m_cible(cible), m_positionActuelle(positionDepart), m_degats(degats),
      m_typeTour(typeTour), m_niveau(niveauTour), m_detruit(false),
      m_sprite(textureParDefaut) 
{
    // Décalage vertical graphique de 16 pixels pour faire apparaître le projectile au sommet de la tour plutôt qu'à sa base
    m_positionActuelle.y -= 16.f; 

    // --- OPTIMISATION IA / RENDEMENT MÉMOIRE ---
    // Les textures sont déclarées en 'static' pour implémenter un Flyweight Pattern implicite.
    // Les fichiers PNG ne sont chargés du disque dur qu'UNE SEULE FOIS, au tout premier tir de la partie.
    static sf::Texture texFeu1, texFeu2, texFeu3;
    static sf::Texture texGlace1, texGlace2, texGlace3;
    static bool initialise = false;

    if (!initialise) {
        // Chargement unique des assets de feu
        bool f1 = texFeu1.loadFromFile("asset/projectile_feu/projectile_feu_1.png");
        bool f2 = texFeu2.loadFromFile("asset/projectile_feu/projectile_feu_4.png");
        bool f3 = texFeu3.loadFromFile("asset/projectile_feu/projectile_feu_6.png");
        
        // Chargement unique des assets de glace
        bool g1 = texGlace1.loadFromFile("asset/projectile_glace/projectile_glace_1.png");
        bool g2 = texGlace2.loadFromFile("asset/projectile_glace/projectile_glace_2.png");
        bool g3 = texGlace3.loadFromFile("asset/projectile_glace/projectile_glace_3.png");

        if (!f1 || !f2 || !f3 || !g1 || !g2 || !g3) {
            std::cerr << "[ERREUR CRITIQUE IA] Impossible de charger un ou plusieurs assets de projectiles dans le dossier asset/ !\n";
        }
        initialise = true;
    }

    float echelle = 0.35f; // Échelle graphique par défaut

    // --- ARBRE DE DÉCISION DE L'IA GRAPHIQUE : SÉLECTION DE LA TEXTURE SELON LE NIVEAU ---
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
            echelle = 0.18f; // Échelle adaptée aux dimensions spécifiques de la texture
        } 
        else if (m_niveau >= 3) {
            m_sprite.setTexture(texGlace3, true);
            echelle = 0.22f; // Évolution visuelle : Gros bloc de cristal final
        }
        else {
            m_sprite.setTexture(texGlace1, true);
            echelle = 0.25f; // Petite comète de glace initiale
        }
    }
    else {
        m_sprite.setTexture(texFeu1, true); // Sécurité anti-crash
    }

    // Centrage automatique du point de pivot (Origin) au milieu de la texture affectée
    auto size = m_sprite.getTexture().getSize(); 
    m_sprite.setOrigin({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f});
    
    m_sprite.setScale({echelle, echelle}); 
    m_sprite.setPosition(m_positionActuelle);
}

void Projectile::update(float deltaTime) {
    if (m_detruit) return;

    // Sécurité : Si le monstre ciblé meurt ou est déréférencé en cours de route, le projectile s'autodétruit
    if (!m_cible || m_cible->isDead()) {
        m_detruit = true;
        return;
    }

    // IA LOGIQUE : Ciblage du torse du squelette (décalage de 24px vers le haut) plutôt que de ses pieds (position 0,0)
    sf::Vector2f posCible = m_cible->getPosition();
    posCible.y -= 24.f; 

    // Calcul du vecteur directionnel et de la distance relative
    sf::Vector2f direction = posCible - m_positionActuelle;
    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);

    // --- GESTION DE LA COLLISION (Seuil d'impact fixé à 10 pixels) ---
    if (distance <= 10.f) {
        m_cible->takeDamage(m_degats); // Application des points de dégâts sur le modèle logique du monstre
        m_detruit = true;              // Flag d'effacement pour le nettoyage automatique dans le main.cpp
        return;
    }

    // --- DÉPLACEMENT AUTO-GUIDÉ ---
    // Normalisation du vecteur de direction pour garantir une vitesse de déplacement linéaire et constante
    sf::Vector2f directionNormalisee = direction / distance;
    m_positionActuelle += directionNormalisee * m_vitesse * deltaTime;
    
    m_sprite.setPosition(m_positionActuelle);

    // --- ORIENTATION VISUELLE AUTOMATIQUE ---
    // Calcule l'angle trigonométrique en radians et le convertit en degrés SFML pour orienter le sprite vers sa cible
    float const PI = 3.14159265f;
    float angle = std::atan2(direction.y, direction.x) * 180.f / PI;
    m_sprite.setRotation(sf::degrees(angle)); // Syntaxe SFML 3 standardisée
}

void Projectile::draw(sf::RenderWindow &window) const {
    if (!m_detruit) {
        window.draw(m_sprite);
    }
}