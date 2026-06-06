#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include "../gestionVague/GestionVague.hpp"
#include "../gestionVague/MonstreInstance.hpp"
#include "../castle/logic/Castle.hpp"
#include "../castle/view/CastleView.hpp"
#include "../Map/Map.hpp"
 
class GameManager {
public:
    GameManager(Map& map);
 
    bool initialiser(const std::string& cheminVagues);
 
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
 
    int getOr() const { return m_orJoueur; }
    bool isGameOver() const { return m_castle.isDestroyed(); }
    bool isVictoire() const { return m_gestionVague.toutesVaguesTerminees() && m_listeMonstres.empty(); }
 
private:
    void mettreAJourMonstres(float deltaTime);
    void nettoyerMonstres();
 
    Map&                        m_map;
    std::vector<sf::Vector2i>   m_chemin;
    std::vector<MonstreInstance> m_listeMonstres;
 
    GestionVague  m_gestionVague;
    Castle        m_castle;
    CastleView    m_castleView;
 
    int m_orJoueur = 100;
};

#endif // GAMEMANAGER_HPP