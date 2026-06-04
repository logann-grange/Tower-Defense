#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "Monster.hpp"    // <-- ADAPTATION : On inclut la vraie classe de base de tes monstres
#include "Projectile.hpp" // <-- Inclut la gestion de la Sprite Sheet des éclats

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
    std::string m_type;  
    std::string m_bonus; 
    int m_valeurEvo;     

    sf::Vector2f m_position; 
    sf::Texture m_texture;   
    sf::Sprite m_sprite;    

    int m_portee;                     
    float m_vitesseAtk;               
    float m_tempsDepuisDerniereAtk{0.0f}; ///< Variable unifiée pour le cooldown de recharge

    std::shared_ptr<Monster> m_cible{nullptr}; // <-- ADAPTATION : Utilise Monster au lieu d'Enemi

public:
    int getPortee() const { return m_portee; }
    sf::Vector2f getPosition() const { return m_position; }

    Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, sf::Vector2f pos, const std::string &texturePath);
    virtual ~Tour() = default;

    // Signature propre et épurée (sans paramètre de texture redondant)
    virtual void attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>> &listeProjectiles);

    // ADAPTATION : La tour prend désormais la liste des "Monster" pour scanner les cibles
    void update(float deltaTime, const std::vector<std::shared_ptr<Monster>> &listeEnemis, std::vector<std::unique_ptr<Projectile>> &listeProjectiles);
    void draw(sf::RenderWindow &window) const;
};