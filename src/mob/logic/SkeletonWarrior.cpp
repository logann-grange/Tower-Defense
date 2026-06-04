#include "SkeletonWarrior.hpp"

SkeletonWarrior::SkeletonWarrior() {
    // --- STATS UNIQUES DU SQUELETTE GUERRIER ---
    m_speed = 60.f;            // Plus lent que le squelette de base
    m_pvMax = 100.f;           // Plus de vie que le squelette de base
    m_pvActuels = m_pvMax;
    m_degatsBase = 2;          // Inflige plus de dégâts au joueur
    m_orRecompense = 30;       // Donne plus d'or à sa mort
    m_faiblesse = "Glace"; // Il explose facilement (sensible aux tours de type feu/bombe)

    m_arrive = false;
    m_directionCourante = Direction::Droite;
} 