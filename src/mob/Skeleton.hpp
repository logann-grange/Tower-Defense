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
    // ❌ CORRECTION : Suppression de "sf::Sprite m_sprite;" d'ici pour utiliser celui de Monster !
    int dmg;
    std::string faiblesse;

    int m_frameLargeur = 64; 
    int m_frameHauteur = 64; 
    int m_nbFramesMax = 6;   
    int m_frameActuelle = 0; 

    float m_tempsAnimation = 0.f;
    float m_vitesseAnimation = 0.1f;
};

#endif // SKELETON_HPP