#pragma once
#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <memory>
#include "../gestionVague/GestionVague.hpp"
#include "../gestionVague/MonstreInstance.hpp"
#include "../castle/logic/Castle.hpp"
#include "../castle/view/CastleView.hpp"
#include "../Map/Map.hpp"
#include "../tower/logic/Projectile.hpp"
#include "../tower/view/ProjectileView.hpp"
#include "../tower/logic/Tour.hpp"
#include "../tower/view/TourView.hpp"
 



struct ProjectileInstance {
    std::unique_ptr<Projectile>    logique;
    std::unique_ptr<ProjectileView> graphique;
};


struct TourInstance {
    Tour    logique;
    TourView graphique;
 
    TourInstance(Tour t, const std::string& texturePath)
        : logique(std::move(t)), graphique(logique, texturePath) {}
};
 

class GameManager {
public:
    GameManager(Map& map);
 
    bool initialiser(const std::string& cheminVagues);
 
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
 
    // Tours
    void placerTour(const Tour& tour, const std::string& texturePath);
 
    int  getOr()       const { return m_orJoueur; }
    void setOr(int or_)      { m_orJoueur = or_; }
    bool isGameOver()  const { return m_castle.isDestroyed(); }
    bool isVictoire()  const { return m_gestionVague.toutesVaguesTerminees() && m_listeMonstres.empty(); }
 
private:
    void mettreAJourMonstres(float deltaTime);
    void mettreAJourTours(float deltaTime);
    void nettoyerMonstres();
 
    Map&                         m_map;
    std::vector<sf::Vector2i>    m_chemin;
    std::vector<MonstreInstance> m_listeMonstres;
    std::vector<TourInstance>    m_listeTours;
    
    GestionVague m_gestionVague;
    Castle       m_castle;
    CastleView   m_castleView;
 
 
    int m_orJoueur = 100;
    std::vector<ProjectileInstance> m_listeProjectiles;
};

#endif // GAMEMANAGER_HPP