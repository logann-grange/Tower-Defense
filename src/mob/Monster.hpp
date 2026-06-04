#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <cmath>
#include <string>
#include <iostream>

class Monster {
public:
    virtual ~Monster() = default;

    enum class Direction { Droite, Gauche };

    void spawn(const std::vector<sf::Vector2i>& chemin);
    void move(float deltaTime);

    void subirDegats(float montant, const std::string& typeDegat);

    bool doitInfligerDegats() {
        if (m_arrive && !m_degatsInfliges && !estMort()) {
            m_degatsInfliges = true;
            return true;
        }
        return false;
    }


    bool estMort() const { return m_pvActuels <= 0.f; }
    float getPvActuels() const { return m_pvActuels; }
    float getPvMax() const { return m_pvMax; }
    int getDegatsBase() const { return m_degatsBase; }
    int getOrRecompense() const { return m_orRecompense; }
    std::string getFaiblesse() const { return m_faiblesse; }

    sf::Vector2f getPosition() const { return m_position; }
    bool estArrive() const { return m_arrive; }
    Direction getDirection() const { return m_directionCourante; }

protected:
    sf::Vector2f m_position;
    float m_speed = 0.f; // Sera modifié par le constructeur du Skeleton ou Goblin

    float m_pvMax = 100.f;
    float m_pvActuels = 100.f;
    int m_degatsBase = 1;         // Le nombre de coeurs/points que le joueur perd si ce monstre arrive au bout
    int m_orRecompense = 0;       // L'or donné au joueur à sa mort
    std::string m_faiblesse = ""; // Exemple: "Feu", "Fleche", "Magie"...
    
    std::vector<sf::Vector2f> m_pointsCheminPixels;
    size_t m_indexEtape = 0;
    bool m_arrive = false;
    bool m_degatsInfliges = false;
    Direction m_directionCourante = Direction::Droite;
};

#endif