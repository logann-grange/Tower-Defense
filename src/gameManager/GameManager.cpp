#include "GameManager.hpp"

GameManager::GameManager(Map& map, SoundManager& soundManager)
    : m_map(map), m_soundManager(soundManager) {
    sf::Vector2i caseDepart = m_map.trouverPointDepartDepuisEntite();
    m_chemin = m_map.genererChemin(caseDepart);
    m_castle.initialiser(m_map.getPositionObjectif());
}

bool GameManager::initialiser(const std::string& cheminVagues) {
    if (!m_gestionVague.loadFromFile(cheminVagues)) {
        std::cerr << "GameManager : Impossible de charger " << cheminVagues << std::endl;
        return false;
    }
    return true;
}

void GameManager::placerTour(const Tour& tour, const std::string& texturePath) {
    // Vérifie que la case est une zone de tour
    sf::Vector2i caseGrille = {
        static_cast<int>(tour.getPosition().x / 16),
        static_cast<int>(tour.getPosition().y / 16)
    };
std::cout << "Position pixels : (" << tour.getPosition().x << ", " << tour.getPosition().y << ")" << std::endl;
std::cout << "Case grille : (" << caseGrille.x << ", " << caseGrille.y << ")" << std::endl;
std::cout << "isTowerZone : " << m_map.isTowerZone(caseGrille.x, caseGrille.y) << std::endl;
std::cout << "Valeur grille : " << m_map.getGridValue(caseGrille.x, caseGrille.y) << std::endl;
std::cout << "Map taille : " << m_map.getWidth() << " x " << m_map.getHeight() << std::endl;
    if (!m_map.isTowerZone(caseGrille.x, caseGrille.y)) {
        std::cout << "Impossible de placer une tour ici !" << std::endl;
        return;
    }
    // Vérifie que le joueur a assez d'or
    if (m_orJoueur < tour.getValeur()) {
        std::cout << "Pas assez d'or !" << std::endl;
        return;
    }
    m_orJoueur -= tour.getValeur();
    m_listeTours.emplace_back(tour, texturePath);
    std::cout << "Tour " << tour.getType() << " placee ! Or restant : " << m_orJoueur << std::endl;
}

void GameManager::update(float deltaTime) {
     std::cout << "Nb tours : " << m_listeTours.size() << std::endl;
    std::cout << "Nb monstres vivants : " << m_listeMonstres.size() << std::endl;
    if (isGameOver()) return;

    m_gestionVague.update(deltaTime, m_listeMonstres, m_chemin);

    if (m_gestionVague.vagueTerminee() && m_listeMonstres.empty()) {
        m_gestionVague.passerVagueSuivante();
    }

    mettreAJourMonstres(deltaTime);
    mettreAJourTours(deltaTime);
    nettoyerMonstres();
}

void GameManager::mettreAJourMonstres(float deltaTime) {
    for (auto& monstre : m_listeMonstres) {
        if (!monstre.logique->estMort()) {
            monstre.logique->move(deltaTime);

            if (monstre.logique->doitInfligerDegats()) {
                m_castle.takeDamage(monstre.logique->getDegatsBase());
                std::cout << "Chateau touche ! PV : " << m_castle.getHealth() << std::endl;
            }
        } else {
            if (!monstre.recompenseDonnee) {
                m_orJoueur += monstre.logique->getOrRecompense();
                monstre.recompenseDonnee = true;
                std::cout << "+" << monstre.logique->getOrRecompense()
                          << " Or ! (Total: " << m_orJoueur << ")" << std::endl;
            }
        }
        monstre.graphique->update(deltaTime, *(monstre.logique));
    }
}

void GameManager::mettreAJourTours(float deltaTime) {
    std::vector<std::shared_ptr<Monster>> listeEnemis;
    for (auto& m : m_listeMonstres) {
        if (!m.logique->estMort())
            listeEnemis.push_back(m.logique);
    }

    // Récupère les nouveaux projectiles créés par les tours
    std::vector<std::unique_ptr<Projectile>> nouveauxProjectiles;
    for (auto& tour : m_listeTours) {
        tour.logique.update(deltaTime, listeEnemis, nouveauxProjectiles);
        tour.graphique.update(tour.logique);
    }

    // Ajoute les nouveaux projectiles avec leur vue
    for (auto& proj : nouveauxProjectiles) {
        // Joue le son au moment de l'apparition
        m_soundManager.playProjectileSound(proj->getType(), proj->getNiveau());

        auto vue = std::make_unique<ProjectileView>(*proj);
        m_listeProjectiles.push_back({std::move(proj), std::move(vue)});
    }

    // Met à jour les projectiles existants
    for (auto& proj : m_listeProjectiles) {
        proj.logique->update(deltaTime);
        proj.graphique->update(*proj.logique);
    }

    // Nettoie les projectiles détruits
    m_listeProjectiles.erase(
        std::remove_if(m_listeProjectiles.begin(), m_listeProjectiles.end(),
            [](const ProjectileInstance& p) { return p.logique->estDetruit(); }),
        m_listeProjectiles.end()
    );
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

void GameManager::reinitialiser(const std::string& cheminVagues) {
    m_listeMonstres.clear();
    m_listeTours.clear();
    m_listeProjectiles.clear();
    m_orJoueur = 100;
    m_gestionVague = GestionVague();
    m_gestionVague.loadFromFile(cheminVagues);
    m_castle.initialiser(m_map.getPositionObjectif());
}

void GameManager::draw(sf::RenderWindow& window) {
    m_castleView.update(m_castle);
    m_castleView.draw(window);

    for (auto& monstre : m_listeMonstres) {
        monstre.graphique->draw(window);
    }

    for (auto& tour : m_listeTours) {
        tour.graphique.draw(window);
    }

    for (auto& proj : m_listeProjectiles) {
    proj.graphique->draw(window);
    }   
}