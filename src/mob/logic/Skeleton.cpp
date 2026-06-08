#include "Skeleton.hpp"

Skeleton::Skeleton() {
    // --- STATS UNIQUES DU SQUELETTE ---
    m_speed = 50.f;
    m_pvMax = 50.f;            // Le squelette a peu de vie
    m_pvActuels = m_pvMax;
    m_degatsBase = 1;          // Si il passe, il inflige 1 dégât au joueur
    m_orRecompense = 30;       // Donne 30 pièces d'or à sa mort
    m_faiblesse = "Feu"; // Il explose facilement (sensible aux tours de type feu/bombe)

    m_arrive = false;
    m_directionCourante = Direction::Droite;
}