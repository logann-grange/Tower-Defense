#pragma once
#include <memory>
#include <string>
#include <SFML/System/Vector2.hpp>

class Monster;
class ProjectileView; // Déclaration anticipée

class Projectile {
private:
    std::shared_ptr<Monster> m_cible;         
    sf::Vector2f m_positionActuelle;            
    int m_degats;                             
    std::string m_typeTour;                   
    bool m_detruit;                           
    int m_niveau;
    float m_vitesse{350.f};

    std::unique_ptr<ProjectileView> m_view; // Lien vers le fichier graphique externe

public:
    Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string& typeTour, int niveauTour);
    virtual ~Projectile();

    void update(float deltaTime);
    bool estDetruit() const { return m_detruit; }
    ProjectileView* getView() const { return m_view.get(); }
};