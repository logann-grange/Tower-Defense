#ifndef SKELETON_HPP
#define SKELETON_HPP

#include <SFML/Graphics.hpp>
#include "Monster.hpp"
#include <string>
#include <vector>
#include <cmath>

class Skeleton : public Monster {
public:
    Skeleton();
    ~Skeleton() override = default;

    void spawn(const std::vector<sf::Vector2i>& cheminPoints);
    void draw(sf::RenderWindow& window);
    void move(float deltaTime) override;
    void mourir() override;
    void attaquer() override;
private:
    std::string lienSprit;
    sf::Vector2f m_positionPixels;       
    std::vector<sf::Vector2i> m_chemin;  
    size_t m_indexEtapeActuelle;
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    int dmg;
    std::string faiblesse;

};

#endif // SKELETON_HPP
