#include "Monster.hpp"

void Monster::spawn(const std::vector<sf::Vector2i>& chemin) {
    m_pointsCheminPixels.clear();
    for (const auto& caseGrille : chemin) {
        m_pointsCheminPixels.push_back({ static_cast<float>(caseGrille.x * 16), static_cast<float>(caseGrille.y * 16) });
    }

    if (!m_pointsCheminPixels.empty()) {
        m_position = m_pointsCheminPixels[0];
        m_indexEtape = 1;
        m_arrive = false;
    }
}

void Monster::move(float deltaTime) {
    if (m_arrive || m_indexEtape >= m_pointsCheminPixels.size()) {
        m_arrive = true;
        return;
    }

    sf::Vector2f cible = m_pointsCheminPixels[m_indexEtape];
    sf::Vector2f direction = cible - m_position;

    if (direction.x < 0.f) {
        m_directionCourante = Direction::Gauche;
    } else if (direction.x > 0.f) {
        m_directionCourante = Direction::Droite;
    }

    float distance = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    float distanceAParcourir = m_speed * deltaTime;

    if (distanceAParcourir >= distance) {
        m_position = cible;
        m_indexEtape++;
    } else {
        m_position += (direction / distance) * distanceAParcourir;
    }
}

void Monster::subirDegats(float montant, const std::string& typeDegat) {
    if (estMort()) return;

    float degatsFinaux = montant;

    // Si la tour utilise un type de dégât auquel le monstre est sensible
    if (typeDegat == m_faiblesse) {
        degatsFinaux = montant * 2.f; // Double dégâts !
        std::cout << "Coup critique ! Faiblesse touchee !" << std::endl;
    }

    m_pvActuels -= degatsFinaux;

    if (m_pvActuels <= 0.f) {
        m_pvActuels = 0.f;
        
    }
}