#include "SkeletonMage.hpp"

SkeletonMage::SkeletonMage() {
    // --- STATS UNIQUES DU SQUELETTE MAGE ---
    m_speed = 60.f;            // Vitesse intermédiaire
    m_pvMax = 150.f;            // Vie intermédiaire
    m_pvActuels = m_pvMax;
    m_degatsBase = 20;          // Inflige plus de dégâts au joueur
    m_orRecompense = 70;       // Donne plus d'or à sa mort
    m_faiblesse = "Feu"; // Il explose facilement (sensible aux tours de type feu/bombe)

    m_arrive = false;
    m_directionCourante = Direction::Droite;
}