#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Monster;

class Projectile {
private:
    std::shared_ptr<Monster> m_cible;         
    sf::Vector2f m_positionActuelle;            
    int m_degats;                             
    std::string m_typeTour;                   
    bool m_detruit;                           
    int m_niveau;

    // ✅ CORRECTION SFML 3 : On initialise le Sprite avec un constructeur vide géré par SFML 3 directement dans le .hpp
    sf::Sprite m_sprite; 
    float m_vitesse{350.f};

public:
    // Le constructeur prend désormais le niveau à la place de l'ancien mode laser
    Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string& typeTour, int niveauTour);

    void update(float deltaTime);
    void draw(sf::RenderWindow &window) const;
    bool estDetruit() const { return m_detruit; }
};