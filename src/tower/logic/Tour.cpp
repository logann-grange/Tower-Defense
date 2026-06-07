#include "Tour.hpp"


Tour::Tour(int id, int atk, int valeur, std::string type, int portee,
           float vitesseAtk, int niveau, sf::Vector2f pos)
    : m_id(id), m_atk(atk), m_valeur(valeur), m_type(type),
      m_portee(portee), m_vitesseAtk(vitesseAtk), m_niveau(niveau),
      m_position(pos), m_valeurEvo(valeur * 0.6f),
      m_tempsDepuisDerniereAtk(0.f)
{}

void Tour::update(float deltaTime,
                  const std::vector<std::shared_ptr<Monster>>& listeEnemis,
                  std::vector<std::unique_ptr<Projectile>>& listeProjectiles)
{
    m_tempsDepuisDerniereAtk += deltaTime;

    float porteeEnPixels   = m_portee * 16.0f;
    float porteeEnPixelsSq = std::pow(porteeEnPixels, 2);

    // Vérification de la cible actuelle
    if (m_cible) {
        if (m_cible->estMort() == true) {
            m_cible = nullptr;
        } else {
            sf::Vector2f posCible = m_cible->getPosition();
            float distSq = std::pow(posCible.x - m_position.x, 2)
                         + std::pow(posCible.y - m_position.y, 2);
            if (distSq > porteeEnPixelsSq)
                m_cible = nullptr;
        }
    }

    // Recherche d'une nouvelle cible
    if (!m_cible) {
        float distanceMinSq = porteeEnPixelsSq;
        for (const auto& enemi : listeEnemis) {
            if (enemi && enemi->estMort() == false) {
                sf::Vector2f posEnemi = enemi->getPosition();
                float distSq = std::pow(posEnemi.x - m_position.x, 2)
                             + std::pow(posEnemi.y - m_position.y, 2);
                if (distSq <= distanceMinSq) {
                    m_cible = enemi;
                    distanceMinSq = distSq;
                }
            }
        }
    }

    // Tir
    if (m_cible && m_tempsDepuisDerniereAtk >= m_vitesseAtk) {
        attaquer(m_cible, listeProjectiles);
        m_tempsDepuisDerniereAtk = 0.f;
    }

    if (m_cible) {
    std::cout << "Cible pos: " << m_cible->getPosition().x << "," << m_cible->getPosition().y << std::endl;
    std::cout << "Tour pos: " << m_position.x << "," << m_position.y << std::endl;
    float porteeEnPixels = m_portee * 16.0f;
    float porteeEnPixelsSq = std::pow(porteeEnPixels, 2);
    sf::Vector2f posCible = m_cible->getPosition();
    float distSq = std::pow(posCible.x - m_position.x, 2) + std::pow(posCible.y - m_position.y, 2);
    std::cout << "distSq: " << distSq << " porteeEnPixelsSq: " << porteeEnPixelsSq << std::endl;
}
}

void Tour::attaquer(std::shared_ptr<Monster> cible,
                    std::vector<std::unique_ptr<Projectile>>& listeProjectiles)                  
{
    if (cible) {
        listeProjectiles.push_back(
            std::make_unique<Projectile>(m_position, cible, m_atk, m_type, m_niveau));
        std::cout << "[TOUR] Tir (" << m_type << ") niveau " << m_niveau << "\n";
        listeProjectiles.push_back(
    std::make_unique<Projectile>(m_position, cible, m_atk, m_type, m_niveau));
    }
}

void Tour::upgrade() {
    if (m_niveau < 3) {
        m_niveau++;
        m_atk += 15;
        std::cout << "[EVOLUTION] Tour " << m_type << " -> niveau " << m_niveau << "\n";
    }
}

void Tour::tenterAmelioration(int& orJoueur) {
    int coutUpgrade = m_niveau * 50;
    if (orJoueur >= coutUpgrade) {
        orJoueur     -= coutUpgrade;
        m_niveau++;
        m_atk         = static_cast<int>(m_atk * 1.30f);
        m_portee      += 25;
        m_vitesseAtk  *= 0.85f;
        std::cout << "Tour amelioree niveau " << m_niveau << "\n";
    } else {
        std::cout << "Pas assez d'or ! Besoin de " << coutUpgrade << "\n";
    }
}
