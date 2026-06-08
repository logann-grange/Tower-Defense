#pragma once
#include <string>
#include <vector>
#include <memory>
#include <SFML/System/Vector2.hpp> // Uniquement pour sf::Vector2f

class Monster;
class Projectile;
class TourView; // Déclaration anticipée de la vue

class Tour
{
protected:
    int m_id;            
    int m_atk;           
    int m_valeur;        
    std::string m_type;  
    std::string m_bonus; 
    int m_valeurEvo;     
    int m_niveau; 

    sf::Vector2f m_position; 
    int m_portee;                     
    float m_vitesseAtk;               
    float m_tempsDepuisDerniereAtk{0.0f}; 

    std::shared_ptr<Monster> m_cible{nullptr}; 
    std::unique_ptr<TourView> m_view; // La logique possède sa vue de manière encapsulée

public:
    int getPortee() const { return m_portee; }
    sf::Vector2f getPosition() const { return m_position; }
    int getNiveau() const { return m_niveau; } 
    std::string getType() const { return m_type; }
    TourView* getView() const { return m_view.get(); }

    Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, int niveau, sf::Vector2f pos, const std::string &texturePath);
    virtual ~Tour(); // Destructeur virtuel obligatoire pour libérer la vue
    
    void update(float deltaTime, const std::vector<std::shared_ptr<Monster>> &listeEnemis, std::vector<std::unique_ptr<Projectile>> &listeProjectiles);
    void upgrade();
    void tenterAmelioration(int& orJoueur);
    virtual void attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>>& listeProjectiles);
};