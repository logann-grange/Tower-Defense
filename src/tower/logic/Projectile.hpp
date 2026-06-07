#pragma once
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <string>

class Monster;

class Projectile {
public:
    Projectile(sf::Vector2f positionDepart, std::shared_ptr<Monster> cible,
               int degats, const std::string& typeTour, int niveauTour);
    virtual ~Projectile() = default;

    void update(float deltaTime);

    bool         estDetruit()      const { return m_detruit; }
    sf::Vector2f getPosition()     const { return m_positionActuelle; }
    float        getAngle()        const { return m_angle; }
    std::string  getType()         const { return m_typeTour; }
    int          getNiveau()       const { return m_niveau; }

protected:
    std::shared_ptr<Monster> m_cible;
    sf::Vector2f             m_positionActuelle;
    int                      m_degats;
    std::string              m_typeTour;
    int                      m_niveau;
    bool                     m_detruit = false;
    float                    m_vitesse = 350.f;
    float                    m_angle   = 0.f;
};
