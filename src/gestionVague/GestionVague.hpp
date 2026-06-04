#ifndef GESTION_VAGUE_HPP
#define GESTION_VAGUE_HPP
#pragma once
#include <string>
#include <vector>
#include <memory>
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include "MonstreInstance.hpp"
#include "../mob/logic/Skeleton.hpp"
#include "../mob/logic/SkeletonWarrior.hpp"
#include "../mob/logic/SkeletonMage.hpp"
#include "../mob/view/SkeletonView.hpp"
#include "../mob/view/SkeletonWarriorView.hpp"
#include "../mob/view/SkeletonMageView.hpp"

 
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
    std::unique_ptr<Monster>     creerLogique(const std::string& type);
    std::unique_ptr<MonsterView> creerGraphique(const std::string& type);
 
    std::vector<Vague> m_vagues;
    int   m_vagueActuelle       = 0;
    int   m_indexProchainMonstre = 0;
    float m_tempsEcoule          = 0.f;
};

#endif // GESTION_VAGUE_HPP