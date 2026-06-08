#pragma once
#include <string>
#include <vector>
#include <memory>
#include <SFML/System/Vector2.hpp> // Léger : Uniquement pour les coordonnées mathématiques sf::Vector2f

// Déclarations anticipées (Forward Declarations) pour éviter les inclusions cycliques
class Monster;
class Projectile;
class TourView; 

/**
 * @class Tour
 * @brief Gère uniquement les calculs de jeu : ciblage, statistiques, cooldowns et économie.
 * Elle ne contient aucun composant graphique direct de la SFML (0% de fenêtres ou de textures).
 */
class Tour
{
protected:
    // --- STATISTIQUES DE JEU ---
    int m_id;            
    int m_atk;           
    int m_valeur;        
    std::string m_type;  
    std::string m_bonus; 
    int m_valeurEvo;     
    int m_niveau; 

    // --- MATHÉMATIQUES DE POSITION ET DE CADENCE ---
    sf::Vector2f m_position; 
    int m_portee; // Portée nominale en nombre de cases                     
    float m_vitesseAtk; // Temps requis entre deux tirs (cooldown)              
    float m_tempsDepuisDerniereAtk{0.0f}; 

    // --- CIBLAGE ---
    std::shared_ptr<Monster> m_cible{nullptr}; // Pointeur vers le monstre actuellement verrouillé

    // --- ENCAPSULATION DE LA VUE ---
    std::unique_ptr<TourView> m_view; // Lien unique vers l'objet qui gère son affichage et ses sons

public:
    // Getters indispensables pour la logique globale et le rendu principal
    int getPortee() const { return m_portee; }
    sf::Vector2f getPosition() const { return m_position; }
    int getNiveau() const { return m_niveau; } 
    std::string getType() const { return m_type; }
    TourView* getView() const { return m_view.get(); } // Permet au main d'accéder au moteur graphique de la tour

    // Constructeur adapté
    Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, int niveau, sf::Vector2f pos, const std::string &texturePath);
    virtual ~Tour(); // Destructeur virtuel obligatoire pour libérer proprement la mémoire de la vue
    
    // Méthodes de calculs logiques
    void update(float deltaTime, const std::vector<std::shared_ptr<Monster>> &listeEnemis, std::vector<std::unique_ptr<Projectile>> &listeProjectiles);
    void upgrade();
    void tenterAmelioration(int& orJoueur);
    virtual void attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>>& listeProjectiles);
};
