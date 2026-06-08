#include "../include/ProjectileView.hpp"

// Texture statique globale de secours pour satisfaire la contrainte SFML 3 à la construction du Sprite
static sf::Texture textureParDefaut;

ProjectileView::ProjectileView(sf::Vector2f positionInitiale, const std::string& typeTour, int niveau) 
    : m_sprite(textureParDefaut) // 🔥 CORRECTION SFML 3 : Liaison initiale avec une texture factice
{
    chargerTexturesEtEchelle(typeTour, niveau);
    m_sprite.setPosition(positionInitiale);
}

void ProjectileView::chargerTexturesEtEchelle(const std::string& typeTour, int niveau) {
    static sf::Texture texFeu1, texFeu2, texFeu3;
    static sf::Texture texGlace1, texGlace2, texGlace3;
    static bool chargees = false;

    if (!chargees) {
        // En SFML 3, loadFromFile est marqué [[nodiscard]], l'assignation à un booléen élimine l'avertissement du compilateur
        bool b1 = texFeu1.loadFromFile("asset/projectile_feu/projectile_feu_1.png");
        bool b2 = texFeu2.loadFromFile("asset/projectile_feu/projectile_feu_4.png");
        bool b3 = texFeu3.loadFromFile("asset/projectile_feu/projectile_feu_6.png");
        bool b4 = texGlace1.loadFromFile("asset/projectile_glace/projectile_glace_1.png");
        bool b5 = texGlace2.loadFromFile("asset/projectile_glace/projectile_glace_2.png");
        bool b6 = texGlace3.loadFromFile("asset/projectile_glace/projectile_glace_3.png");
        chargees = (b1 && b2 && b3 && b4 && b5 && b6);
    }

    float echelle = 0.35f; 

    if (typeTour == "Feu") {
        if (niveau == 2) { m_sprite.setTexture(texFeu2, true); echelle = 0.45f; } 
        else if (niveau >= 3) { m_sprite.setTexture(texFeu3, true); echelle = 0.30f; }
        else { m_sprite.setTexture(texFeu1, true); echelle = 0.35f; }
    } 
    else if (typeTour == "Glace") {
        if (niveau == 2) { m_sprite.setTexture(texGlace2, true); echelle = 0.18f; } 
        else if (niveau >= 3) { m_sprite.setTexture(texGlace3, true); echelle = 0.22f; }
        else { m_sprite.setTexture(texGlace1, true); echelle = 0.25f; }
    } else {
        m_sprite.setTexture(texFeu1, true);
    }

    auto size = m_sprite.getTexture().getSize(); 
    m_sprite.setOrigin({static_cast<float>(size.x) / 2.f, static_cast<float>(size.y) / 2.f});
    m_sprite.setScale({echelle, echelle});
}

void ProjectileView::synchroniser(sf::Vector2f nouvellePosition, float angleDegres) {
    m_sprite.setPosition(nouvellePosition);
    m_sprite.setRotation(sf::degrees(angleDegres));
}

void ProjectileView::draw(sf::RenderWindow &window) const {
    window.draw(m_sprite);
}