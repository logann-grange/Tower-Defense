#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map/Map.hpp"
#include "mob/logic/Skeleton.hpp"
#include "mob/view/SkeletonView.hpp" // <-- On inclut le fichier graphique

int main() {
    sf::RenderWindow window(sf::VideoMode({1032, 792}), "Tower Defence - Propre !");

    Map mon_niveau;
    if (!mon_niveau.loadFromFile("Map/map_tower_defence.ldtk")) {
        return -1;
    }

    sf::View vueJeu(sf::FloatRect({0.f, 0.f}, {688.f, 528.f}));
    window.setView(vueJeu);

    sf::Vector2i caseDepart = mon_niveau.trouverPointDepartDepuisEntite(); 
    std::vector<sf::Vector2i> cheminMonstres = mon_niveau.genererChemin(caseDepart);

    // --- INSTANCIATION SÉPARÉE ---
    Skeleton squeletteLogique;              // La logique pure
    SkeletonView squeletteGraphique;        // Le visuel pur
    
    squeletteLogique.spawn(cheminMonstres); // On lance la logique au départ

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        // --- MISE À JOUR LOGIQUE ---
        squeletteLogique.move(deltaTime); 

        // --- MISE À JOUR GRAPHIQUE ---
        // La vue a besoin du deltaTime pour son animation, et de la logique pour copier sa position
        squeletteGraphique.update(deltaTime, squeletteLogique); 

        // --- AFFICHAGE ---
        window.clear();
        
        mon_niveau.draw(window); 
        squeletteGraphique.draw(window); // On dessine l'objet graphique

        window.display();
    }

    return 0;
}