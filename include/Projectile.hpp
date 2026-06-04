#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Monster;

// 💡 AJOUT : Définition des deux modes de tir possibles
enum class ModeTir {
    Espace,    // Le laser flash brièvement (0.12s) puis s'éteint
    Continu    // Le laser reste tant que le monstre est vivant et à portée
};

class Projectile {
private:
    sf::RectangleShape m_laserLine;           
    std::shared_ptr<Monster> m_cible;         
    sf::Vector2f m_positionDepart;            
    
    int m_degats;                             
    std::string m_typeTour;                   
    bool m_detruit;                           
    ModeTir m_mode; // 💡 Stocke le mode choisi (Espace ou Continu)

    float m_dureeVieActuelle{0.0f};
    const float m_dureeVieMax{0.12f};         

public:
    // 💡 Le constructeur accepte désormais le ModeTir en paramètre
    Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string& typeTour, ModeTir mode);

    void update(float deltaTime);
    void draw(sf::RenderWindow &window) const;
    bool estDetruit() const { return m_detruit; }
};