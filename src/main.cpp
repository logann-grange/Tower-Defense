#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include "Map.hpp"
#include "mob/Skeleton.hpp"
#include "../include/Tour.hpp"
#include "Projectile.hpp" 

int main()
{
    // Configuration de la fenêtre SFML 3
    sf::RenderWindow window(sf::VideoMode({1280, 992}), "Tower Defence - Vagues de Squelettes");

    Map mon_niveau;
    if (!mon_niveau.loadFromFile("Map/map_tower_defence.ldtk"))
    {
        return -1;
    }

    // Vue de jeu pour zoomer sur la carte 16x16
    sf::FloatRect zoneVue({0.f, 0.f}, {640.f, 496.f});
    sf::View vueJeu(zoneVue);
    window.setView(vueJeu);

    // 1. GENERER LE CHEMIN DU SQUELETTE
    sf::Vector2i caseDepart = mon_niveau.trouverPointDepartDepuisEntite();
    if (caseDepart.x == 0 && caseDepart.y == 0)
    {
        caseDepart = sf::Vector2i(0, 5);
    }

    std::vector<sf::Vector2i> cheminMonstres = mon_niveau.genererChemin(caseDepart);

    // 2. CREER LE SQUELETTE VIA UN POINTEUR INTELLIGENT (shared_ptr)
    auto monSquelette = std::make_shared<Skeleton>();
    monSquelette->spawn(cheminMonstres);

    // Tableau requis pour la fonction update de ta tour
    std::vector<std::shared_ptr<Monster>> listeMonstres;
    listeMonstres.push_back(monSquelette);

    // Tableau qui va stocker les projectiles générés lors des tirs
    std::vector<std::unique_ptr<Projectile>> listeProjectiles;

    // 3. CREER LA TOUR DE GLACE (Case X=23, Y=12 pour ne pas la superposer à l'autre)
    sf::Vector2f positionTourPixels(25 * 16.f + 8.f, 12 * 16.f + 8.f);
    Tour maTourGlace(1, 15, 150, "Glace", 4, 0.5f, 1, positionTourPixels, "asset/tour_glace.png");

    // 4. CREER LA TOUR DE FEU (Case X=25, Y=12)
    // // ✅ ACTIVATION & CORRECTION : Décommentée et nettoyée du paramètre "modeTir"
    sf::Vector2f positionTourFeuPixels(25 * 16.f + 8.f, 12 * 16.f + 8.f);
    Tour maTourFeu(2, 15, 150, "Feu", 4, 0.5f, 3, positionTourFeuPixels, "asset/tour_feu.png");

    sf::Clock clock;

    // Boucle principale du jeu
    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }

        // --- ⚙️ LOGIQUE / MISE A JOUR DES OBJETS ---

        // Fait avancer le squelette s'il est vivant
        if (!monSquelette->isDead())
        {
            monSquelette->move(deltaTime);
        }

        // Mise à jour des tours
        maTourGlace.update(deltaTime, listeMonstres, listeProjectiles);
        maTourFeu.update(deltaTime, listeMonstres, listeProjectiles); // ✅ Activée !

        // Mise à jour des projectiles (boules de feu, etc.) et nettoyage
        for (auto it = listeProjectiles.begin(); it != listeProjectiles.end();)
        {
            (*it)->update(deltaTime);
            if ((*it)->estDetruit())
            { 
                it = listeProjectiles.erase(it);
            }
            else
            {
                ++it;
            }
        }

        // --- 🎨 DESSIN (RENDU GRAPHIQUE) ---
        window.clear();

        // 1. Dessin du sol
        mon_niveau.draw(window);

        // 2. Dessin des tours
        maTourGlace.draw(window);
        maTourFeu.draw(window); // ✅ Activée !

        // 3. Dessin du monstre
        if (!monSquelette->isDead())
        {
            monSquelette->draw(window);
        }

        // 4. Dessin des projectiles physiques par-dessus tout le monde
        for (const auto &projectile : listeProjectiles)
        {
            projectile->draw(window);
        }

        window.display();
    }

    return 0;
}