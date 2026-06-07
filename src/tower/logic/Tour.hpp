#pragma once
#include <SFML/System/Vector2.hpp>
#include <string>
#include <vector>
#include <memory>
#include "mob/Monster.hpp"
#include "Projectile.hpp"
#include <cmath>
#include <iostream>
#include "ProjectileFeu.hpp"
#include "ProjectileGlace.hpp"

class Monster;
class Projectile;

class Tour {
public:
    Tour() = default;
    Tour(int id, int atk, int valeur, std::string type, int portee,
         float vitesseAtk, int niveau, sf::Vector2f pos);

    void update(float deltaTime,
                const std::vector<std::shared_ptr<Monster>>& listeEnemis,
                std::vector<std::unique_ptr<Projectile>>& listeProjectiles);

    void upgrade();
    void tenterAmelioration(int& orJoueur);

    // Getters
    int              getId()       const { return m_id; }
    int              getAtk()      const { return m_atk; }
    int              getValeur()   const { return m_valeur; }
    std::string      getType()     const { return m_type; }
    int              getPortee()   const { return m_portee; }
    float            getVitesse()  const { return m_vitesseAtk; }
    int              getNiveau()   const { return m_niveau; }
    sf::Vector2f     getPosition() const { return m_position; }

protected:
    int          m_id             = 0;
    int          m_atk            = 0;
    int          m_valeur         = 0;
    std::string  m_type           = "";
    int          m_portee         = 0;
    float        m_vitesseAtk     = 0.f;
    int          m_niveau         = 1;
    sf::Vector2f m_position       = {0.f, 0.f};
    int          m_valeurEvo      = 0;
    float        m_tempsDepuisDerniereAtk = 0.f;

    std::shared_ptr<Monster> m_cible = nullptr;

private:
    void attaquer(std::shared_ptr<Monster> cible,
                  std::vector<std::unique_ptr<Projectile>>& listeProjectiles);
};
