#include "GameManager.hpp"

GameManager::GameManager(Map& map) : m_map(map) {
    // Récupère le chemin depuis la map
    sf::Vector2i caseDepart = m_map.trouverPointDepartDepuisEntite();
    m_chemin = m_map.genererChemin(caseDepart);

    // Place le château sur la position de l'objectif
    m_castle.initialiser(m_map.getPositionObjectif());
}

bool GameManager::initialiser(const std::string& cheminVagues) {
    if (!m_gestionVague.loadFromFile(cheminVagues)) {
        std::cerr << "GameManager : Impossible de charger " << cheminVagues << std::endl;
        return false;
    }
    return true;
}

void GameManager::update(float deltaTime) {
    if (isGameOver()) return;

    // Spawn des monstres
    m_gestionVague.update(deltaTime, m_listeMonstres, m_chemin);

    // Passe à la vague suivante si tout est mort
    if (m_gestionVague.vagueTerminee() && m_listeMonstres.empty()) {
        m_gestionVague.passerVagueSuivante();
    }

    // Mise à jour logique et visuelle des monstres
    mettreAJourMonstres(deltaTime);

    // Nettoyage
    nettoyerMonstres();
}

void GameManager::mettreAJourMonstres(float deltaTime) {
    for (auto& monstre : m_listeMonstres) {
        if (!monstre.logique->estMort()) {
            monstre.logique->move(deltaTime);

            if (monstre.logique->doitInfligerDegats()) {
                m_castle.takeDamage(monstre.logique->getDegatsBase());
                std::cout << "Aie ! Chateau touche ! PV restants : " 
                          << m_castle.getHealth() << std::endl;
                if (isGameOver()) {
                    std::cout << "Game Over !" << std::endl;
                }
            }
        } else {
            if (!monstre.recompenseDonnee) {
                m_orJoueur += monstre.logique->getOrRecompense();
                monstre.recompenseDonnee = true;
                std::cout << "Monstre tue ! +" << monstre.logique->getOrRecompense()
                          << " Or. (Total: " << m_orJoueur << ")" << std::endl;
            }
        }

        monstre.graphique->update(deltaTime, *(monstre.logique));
    }
}

void GameManager::nettoyerMonstres() {
    for (auto it = m_listeMonstres.begin(); it != m_listeMonstres.end(); ) {
        if (it->logique->estMort() && it->graphique->estPretADetruire()) {
            it = m_listeMonstres.erase(it);
        } else if (it->logique->estArrive()) {
            it = m_listeMonstres.erase(it);
        } else {
            ++it;
        }
    }
}


void GameManager::draw(sf::RenderWindow& window) {
    m_castleView.update(m_castle);
    m_castleView.draw(window);

    for (auto& monstre : m_listeMonstres) {
        monstre.graphique->draw(window);
    }
}