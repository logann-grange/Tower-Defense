#pragma once
#include <memory>
#include <string>
#include <SFML/System/Vector2.hpp> // Uniquement pour manipuler les positions sf::Vector2f

class Monster;
class ProjectileView; // Déclaration anticipée

/**
 * @class Projectile
 * @brief Calcule la trajectoire mathématique, la vitesse, le ciblage et l'application 
 * des dégâts sur les monstres. Elle délègue tout son affichage à ProjectileView.
 */
class Projectile {
private:
    // --- VARIABLES DE VAGUE ET DE STATS ---
    std::shared_ptr<Monster> m_cible;         
    sf::Vector2f m_positionActuelle;            
    int m_degats;                             
    std::string m_typeTour;                   
    bool m_detruit;                           
    int m_niveau;
    float m_vitesse{350.f}; // Vitesse de déplacement uniforme en pixels/sec

    // --- ENCAPSULATION DE LA VUE ---
    std::unique_ptr<ProjectileView> m_view; // Gestionnaire d'affichage autonome du projectile

public:
    Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string& typeTour, int niveauTour);
    virtual ~Projectile();

    void update(float deltaTime);
    bool estDetruit() const { return m_detruit; }
    ProjectileView* getView() const { return m_view.get(); } // Accès pour la boucle de rendu du main
};
