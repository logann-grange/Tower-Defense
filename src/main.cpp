#include <SFML/Graphics.hpp>
#include <iostream>
#include "Map/Map.hpp"
#include "./gameManager/GameManager.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({1376, 1056}), "Tower Defence");

    Map mon_niveau;
    if (!mon_niveau.loadFromFile("Map/map_tower_defence.ldtk")) {
        std::cerr << "Erreur : Impossible de charger la carte !" << std::endl;
        return -1;
    }

    sf::View vueJeu(sf::FloatRect({0.f, 0.f}, {688.f, 528.f}));
    window.setView(vueJeu);

    GameManager gameManager(mon_niveau);
    if (!gameManager.initialiser("./data/Vague.json")) {
        return -1;
    }

    sf::Clock clock;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
        }

        if (!gameManager.isGameOver()) {
            gameManager.update(deltaTime);
        }

        if (gameManager.isVictoire()) {
            std::cout << "Victoire !" << std::endl;
            window.close();
        }

        window.clear(sf::Color(40, 44, 52));
        window.setView(vueJeu);
        mon_niveau.draw(window);
        gameManager.draw(window);
        window.display();
    }

    return 0;
}