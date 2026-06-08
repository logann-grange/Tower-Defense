#pragma once
#include <SFML/Graphics.hpp>
#include <string>

/**
 * @class ProjectileView
 * @brief Encapsule et affiche les images de projectiles. Utilise un cache de textures statiques
 * pour optimiser le GPU et ne pas recharger les fichiers de textures à chaque tir.
 */
class ProjectileView {
private:
    sf::Sprite m_sprite;

    // Méthode interne pour attribuer l'image et l'échelle correspondantes
    void chargerTexturesEtEchelle(const std::string& typeTour, int niveau);

public:
    ProjectileView(sf::Vector2f positionInitiale, const std::string& typeTour, int niveau);
    
    void synchroniser(sf::Vector2f nouvellePosition, float angleDegres); // Fait le pont avec les calculs logiques
    void draw(sf::RenderWindow &window) const;
};
