#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map.hpp"
#include "mob/Skeleton.hpp"

int main() {
    // CORRECTION SFML 3 : VideoMode prend maintenant un sf::Vector2u pour la taille !
    sf::RenderWindow window(sf::VideoMode({1280, 992}), "Tower Defence - Vagues de Squelettes");

    Map mon_niveau;
    if (!mon_niveau.loadFromFile("Map/map_tower_defence.ldtk")) {
        return -1;
    }

    // CORRECTION SFML 3 : FloatRect prend désormais ({position_x, position_y}, {largeur, hauteur})
    sf::View vueJeu(sf::FloatRect({0.f, 0.f}, {640.f, 496.f}));
    window.setView(vueJeu);

    // 1. GENERER LE CHEMIN AUTOMATIQUEMENT VIA L'ENTITÉ
    sf::Vector2i caseDepart = mon_niveau.trouverPointDepartDepuisEntite(); 
    std::cout << "DEBUG LOG: Position depart : (" << caseDepart.x << ", " << caseDepart.y << ")\n";

    std::vector<sf::Vector2i> cheminMonstres = mon_niveau.genererChemin(caseDepart);
    std::cout << "DEBUG LOG: Taille du chemin genere = " << cheminMonstres.size() << " cases." << std::endl;

    // 2. CREER LE SQUELETTE
    Skeleton monSquelette;
    monSquelette.spawn(cheminMonstres); 

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        // --- MISE À JOUR LOGIQUE ---
        monSquelette.move(deltaTime); 

        // --- AFFICHAGE ---
        window.clear();
        
        mon_niveau.draw(window); 
        monSquelette.draw(window); 

        window.display();
    }

    return 0;
}