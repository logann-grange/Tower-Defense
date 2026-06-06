#ifndef GESTION_VAGUE_HPP
#define GESTION_VAGUE_HPP
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <SFML/System/Vector2.hpp>
#include "MonstreInstance.hpp"
#include <map>
#include "MonsterFactory.hpp"
 
struct MonstreASpawner {
    std::string type;
    float spawnTime;
};
 
struct Vague {
    int id;
    std::vector<MonstreASpawner> monstres;
};
 
class GestionVague {
public:
    GestionVague();
    bool loadFromFile(const std::string& path);
 
    void update(float deltaTime,
                std::vector<MonstreInstance>& listeMonstres,
                const std::vector<sf::Vector2i>& chemin);
 
    bool vagueTerminee() const;       // true quand tous les monstres de la vague ont spawné
    bool toutesVaguesTerminees() const;
    int  getNumVagueActuelle() const { return m_vagueActuelle + 1; }
    int  getNbVagues() const { return static_cast<int>(m_vagues.size()); }
 
    void passerVagueSuivante();       // à appeler quand listeMonstres est vide ET vagueTerminee()
 
private:
    std::map<std::string, std::unique_ptr<MonstreFactory>> m_factories;
 
 
    std::vector<Vague> m_vagues;
    int   m_vagueActuelle       = 0;
    int   m_indexProchainMonstre = 0;
    float m_tempsEcoule          = 0.f;
};

#endif // GESTION_VAGUE_HPP