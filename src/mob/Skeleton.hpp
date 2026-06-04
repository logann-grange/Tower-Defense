#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <SFML/Graphics.hpp>
#include "Monster.hpp"
#include <string>
#include <vector>
#include <cmath>

class Skeleton : public Monster
{
public:
    Skeleton();
    ~Skeleton() override = default;

    void spawn(const std::vector<sf::Vector2i> &cheminPoints);
    void draw(sf::RenderWindow &window);
    void move(float deltaTime) override;
    void mourir() override;
    void attaquer() override;
    // 💡 AJOUT : On remplace la fonction par les vrais pixels ultra-précis du squelette
    sf::Vector2f getPosition() const override
    {
        return m_positionPixels;
    }

private:
    std::string lienSprit;
    sf::Vector2f m_positionPixels;
    std::vector<sf::Vector2i> m_chemin;
    size_t m_indexEtapeActuelle;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    int dmg;
    std::string faiblesse;

    int m_frameLargeur = 64; // À changer en 32 si ton sprite fait 32x32 !
    int m_frameHauteur = 64; // À changer en 32 si ton sprite fait 32x32 !
    int m_nbFramesMax = 6;   // Ton image contient 8 squelettes horizontaux
    int m_frameActuelle = 0; // L'index de l'image en cours (0 à 7)

    float m_tempsAnimation = 0.f;
    float m_vitesseAnimation = 0.1f;
};

#endif // SKELETON_HPP
