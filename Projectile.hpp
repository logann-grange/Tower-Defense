#pragma once
#include <SFML/Graphics.hpp>
#include <memory>
#include <string>

class Monster; // Déclaration anticipée (Forward Declaration) pour éviter les inclusions cycliques

/**
 * @class Projectile
 * @brief Classe gérant le déplacement physique, l'orientation visuelle et l'application des dégâts d'un projectile autoguidé.
 * @note Version optimisée SFML 3. Les textures sont partagées statiquement pour éviter les rechargements de disques inutiles.
 */
class Projectile {
private:
    std::shared_ptr<Monster> m_cible;         ///< Pointeur partagé vers le monstre ciblé
    sf::Vector2f m_positionActuelle;          ///< Position 2D actuelle du projectile (en pixels)
    int m_degats;                             ///< Quantité de dégâts à infliger lors de la collision
    std::string m_typeTour;                   ///< Type élémentaire hérité de la tour ("Feu", "Glace")
    bool m_detruit;                           ///< Flag d'état : vrai si le projectile doit être nettoyé du vecteur global
    int m_niveau;                             ///< Niveau hérité de la tour pour déterminer la taille et l'image du sprite

    sf::Sprite m_sprite;                      ///< Sprite SFML 3 pour l'affichage du projectile
    float m_vitesse{350.f};                   ///< Vitesse de déplacement rectiligne (pixels par seconde)

public:
    /**
     * @brief Constructeur du Projectile
     * @param positionDepart Coordonnées initiales (généralement le sommet de la tourelle)
     * @param cible Pointeur intelligent vers la cible en mouvement
     * @param degats Valeur des dégâts de la tour au moment du tir
     * @param typeTour Chaîne représentant le type élémentaire de la tour
     * @param niveauTour Niveau actuel de la tour (1, 2 ou 3) servant au choix visuel de l'asset
     */
    Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible, int degats, const std::string& typeTour, int niveauTour);

    /**
     * @brief Met à jour le déplacement du projectile vers sa cible, gère l'auto-guidage, le calcul d'angle et les collisions.
     * @param deltaTime Temps écoulé en secondes (permet un déplacement fluide indépendant du framerate)
     */
    void update(float deltaTime);

    /**
     * @brief Effectue le rendu graphique du projectile dans la fenêtre de jeu si celui-ci n'est pas détruit.
     * @param window Référence vers la fenêtre SFML active
     */
    void draw(sf::RenderWindow &window) const;

    /**
     * @brief Getter renvoyant l'état de destruction du projectile.
     * @return true si le projectile a touché ou perdu sa cible et doit être supprimé de la mémoire.
     */
    bool estDetruit() const { return m_detruit; }
};