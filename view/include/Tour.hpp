#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include <optional>
// #include "Monster.hpp"    
// #include "Projectile.hpp" 

/**
 * @class Tour
 * @brief Classe maîtresse gérant l'affichage des tourelles, la détection de portée et la cadence de tir.
 */
class Tour
{
protected:
    int m_id;            
    int m_atk;           
    int m_valeur;        
    std::string m_bonus;
    int m_valeurEvo;     

    sf::Vector2f m_position; 
    sf::Texture m_texture;   
        

    int m_portee;                     
    float m_vitesseAtk;               
    float m_tempsDepuisDerniereAtk{0.0f}; 

    //std::shared_ptr<Monster> m_cible{nullptr}; 
    int m_niveau; 

public:
    std::string m_type;  
    std::optional<sf::Sprite> m_sprite;
    int getPortee() const { return m_portee; }
    sf::Vector2f getPosition() const { return m_position; }
    int getNiveau() const { return m_niveau; } 

    Tour();
    Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, int niveau, sf::Vector2f pos, const std::string &texturePath);
    virtual ~Tour() = default;
    
    void upgrade();
    //virtual void attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>>& listeProjectiles);
    //void update(float deltaTime, const std::vector<std::shared_ptr<Monster>> &listeEnemis, std::vector<std::unique_ptr<Projectile>> &listeProjectiles);
    void draw(sf::RenderWindow &window) const;
    //std::string getType() const { return m_type; }
};