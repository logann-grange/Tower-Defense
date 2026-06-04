#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "Map.hpp"
#include "mob/Skeleton.hpp"
#include "../include/Tour.hpp" 
#include "Projectile.hpp" // <-- AJOUT INDISPENSABLE pour les lasers

int main() {
    // Configuration de la fenêtre SFML 3
    sf::RenderWindow window(sf::VideoMode({1280, 992}), "Tower Defence - Vagues de Squelettes");

    Map mon_niveau;
    if (!mon_niveau.loadFromFile("Map/map_tower_defence.ldtk")) {
        return -1;
    }

    // Vue de jeu pour zoomer sur la carte 16x16
    sf::FloatRect zoneVue({0.f, 0.f}, {640.f, 496.f});
    sf::View vueJeu(zoneVue);
    window.setView(vueJeu);

    // 1. GENERER LE CHEMIN DU SQUELETTE
    sf::Vector2i caseDepart = mon_niveau.trouverPointDepartDepuisEntite(); 
    if (caseDepart.x == 0 && caseDepart.y == 0) {
        caseDepart = sf::Vector2i(0, 5); 
    }

    std::vector<sf::Vector2i> cheminMonstres = mon_niveau.genererChemin(caseDepart);

    // 2. CREER LE SQUELETTE VIA UN POINTEUR INTELLIGENT (shared_ptr)
    // C'est obligatoire pour que m_cible (qui est un shared_ptr<Monster>) puisse le stocker !
    auto monSquelette = std::make_shared<Skeleton>();
    monSquelette->spawn(cheminMonstres); 

    // Tableau requis pour la fonction update de ta tour
    std::vector<std::shared_ptr<Monster>> listeMonstres;
    listeMonstres.push_back(monSquelette);

    // Tableau qui va stocker les lasers générés lors des tirs
    std::vector<std::unique_ptr<Projectile>> listeProjectiles;

    // 3. CREER LA TOUR DE GLACE SUR TON RECTANGLE NOIR (Case X=23, Y=12)
    sf::Vector2f positionTourPixels(23 * 16.f + 8.f, 12 * 16.f + 8.f);
    Tour maTourGlace(1, 15, 100, "Glace", 180, 1.5f, positionTourPixels, "asset/tour_glace.png");

    sf::Clock clock;

    // Boucle principale du jeu
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) {
                window.close();
            }
        }

        // --- ⚙️ LOGIQUE / MISE A JOUR DES OBJETS ---
        
        // Fait avancer le squelette s'il est vivant
        if (!monSquelette->isDead()) {
            monSquelette->move(deltaTime);
        }

        // ✅ CORRECTION 1 : On appelle enfin l'update de ta tour à chaque frame !
        maTourGlace.update(deltaTime, listeMonstres, listeProjectiles);

        // ✅ CORRECTION 2 : On met à jour les lasers et on supprime ceux qui ont fini de flasher
        for (auto it = listeProjectiles.begin(); it != listeProjectiles.end();) {
            (*it)->update(deltaTime);
            if ((*it)->estDetruit()) { // Nettoyage automatique des anciens tirs
                it = listeProjectiles.erase(it);
            } else {
                ++it;
            }
        }

        // --- 🎨 DESSIN (RENDU GRAPHIQUE) ---
        window.clear();

        // 1. Dessin du sol
        mon_niveau.draw(window);

        // 2. Dessin de la tour
        maTourGlace.draw(window);

        // 3. Dessin du monstre
        if (!monSquelette->isDead()) {
            monSquelette->draw(window);
        }

        // ✅ CORRECTION 3 : Dessin des rayons lasers à l'écran par-dessus tout le monde
        for (const auto& laser : listeProjectiles) {
            laser->draw(window);
        }

        window.display();
    }

    return 0;
}