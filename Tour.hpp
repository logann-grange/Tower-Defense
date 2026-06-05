#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <memory>
#include "Monster.hpp"    
#include "Projectile.hpp" 

/**
 * @class Tour
 * @brief Classe maîtresse gérant l'affichage des tourelles, la détection de portée, la recherche de cibles et le système d'amélioration.
 * @note Version compatible SFML 3. Conçue pour une intégration modulaire dans n'importe quelle boucle de jeu principale.
 */
class Tour
{
protected:
    int m_id;                             ///< Identifiant unique de la tour
    int m_atk;                            ///< Points de dégâts infligés par tir
    int m_valeur;                         ///< Coût d'achat initial de la tour
    std::string m_type;                   ///< Élément ou type de la tour (ex: "Feu", "Glace")
    std::string m_bonus;                  ///< Effet ou attribut bonus optionnel
    int m_valeurEvo;                      ///< Valeur de revente ou coût de base d'évolution

    sf::Vector2f m_position;              ///< Position centrale de la tour sur la carte (en pixels)
    sf::Texture m_texture;                ///< Texture SFML 3 de la tourelle
    sf::Sprite m_sprite;                  ///< Sprite SFML 3 pour le rendu visuel

    int m_portee;                         ///< Rayon d'action de la tour (exprimé en NOMBRE DE CASES de 16x16)
    float m_vitesseAtk;                   ///< Cadence de tir : temps d'attente requis entre deux attaques (en secondes)
    float m_tempsDepuisDerniereAtk{0.0f}; ///< Accumulateur de temps (DeltaTime) pour le cooldown de tir

    std::shared_ptr<Monster> m_cible{nullptr}; ///< Pointeur intelligent vers le monstre actuellement ciblé
    int m_niveau;                         ///< Niveau actuel de la tour (commence à 1, max 3)

public:
    // --- ACCESSEURS / GETTERS (Pour l'interface ou la logique globale) ---
    int getPortee() const { return m_portee; }
    sf::Vector2f getPosition() const { return m_position; }
    int getNiveau() const { return m_niveau; } 
    std::string getType() const;

    /**
     * @brief Constructeur de la classe Tour (Standardisé SFML 3)
     * @param id Identifiant de la tour
     * @param atk Dégâts initiaux
     * @param valeur Prix d'achat
     * @param type Chaîne identifiant l'élément ("Feu", "Glace")
     * @param portee Portée de la tour en nombre de cases
     * @param vitesseAtk Cooldown de tir en secondes
     * @param niveau Niveau de départ (généralement 1)
     * @param pos Position de placement sur la carte
     * @param texturePath Chemin d'accès relatif vers l'asset image (.png)
     */
    Tour(int id, int atk, int valeur, std::string type, int portee, float vitesseAtk, int niveau, sf::Vector2f pos, const std::string &texturePath);
    
    virtual ~Tour() = default;
    
    /**
     * @brief Méthode d'évolution interne basique (augmente le niveau et l'attaque sans condition d'or)
     */
    void upgrade();

    /**
     * @brief Instancie un nouveau Projectile dirigé vers la cible et l'ajoute au conteneur global.
     * @param cible Le monstre partagé à attaquer
     * @param listeProjectiles Référence vers le vecteur de projectiles du main.cpp pour y injecter le tir
     */
    virtual void attaquer(std::shared_ptr<Monster> cible, std::vector<std::unique_ptr<Projectile>>& listeProjectiles);

    /**
     * @brief Gère toute la logique de la tour par frame (Cooldown, perte de cible, détection du monstre le plus proche).
     * @param deltaTime Temps écoulé depuis la frame précédente (clock.restart().asSeconds())
     * @param listeEnemis Vecteur contenant les pointeurs de tous les monstres en vie sur la map
     * @param listeProjectiles Référence vers le conteneur de projectiles du main.cpp
     */
    void update(float deltaTime, const std::vector<std::shared_ptr<Monster>> &listeEnemis, std::vector<std::unique_ptr<Projectile>> &listeProjectiles);

    /**
     * @brief Effectue l'affichage graphique du sprite de la tour à l'écran.
     * @param window Référence vers la fenêtre de rendu SFML
     */
    void draw(sf::RenderWindow &window) const;

    /**
     * @brief Système d'évolution interactif lié à l'économie du joueur.
     * @details Vérifie si l'or du joueur est suffisant, déduit le coût calculé dynamiquement,
     * monte le niveau et applique un bonus majeur aux statistiques (Attaque, Portée, Cadence).
     * @param orJoueur Référence vers l'or global du joueur (modifié directement en cas de succès)
     */
    void tenterAmelioration(int& orJoueur);
};