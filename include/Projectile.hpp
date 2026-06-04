#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

// Déclaration anticipée pour éviter les inclusions cycliques
class Monster;

/**
 * @class Projectile
 * @brief Gère l'affichage et l'impact d'un rayon laser reliant la tour à sa cible.
 */
class Projectile {
private:
    // Remplacement des sprites par un rectangle épais pour dessiner le laser
    sf::RectangleShape m_laserLine;           ///< Composant graphique du rayon laser
    
    std::shared_ptr<Monster> m_cible;         ///< Pointeur vers le monstre ciblé
    sf::Vector2f m_positionDepart;            ///< Position de la tour émettrice
    
    int m_degats;                             ///< Dégâts infligés par le laser
    std::string m_typeTour;                   ///< Type de la tour (ex: "Glace")
    bool m_detruit;                           ///< État d'activation du laser

    // Paramètres de persistance du laser à l'écran
    float m_dureeVieActuelle{0.0f};
    const float m_dureeVieMax{0.12f};         ///< Durée d'affichage du rayon en secondes

public:
    /**
     * @brief Constructeur du laser reliant la tour à un monstre
     */
    Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string& typeTour);

    /**
     * @brief Met à jour l'alignement du laser sur le monstre et gère son extinction
     */
    void update(float deltaTime);

    /**
     * @brief Dessine la ligne de laser à l'écran
     */
    void draw(sf::RenderWindow& window) const;

    /**
     * @brief Indique si le laser s'est éteint
     */
    bool estDetruit() const { return m_detruit; }
}; // Le point-virgule ici est obligatoire en C++ !