#include <SFML/Graphics.hpp>
#include "view/include/bouton.hpp"
#include "view/include/menu.hpp"
#include "controller/include/controlManager.hpp"
#include "view/include/param.hpp"
#include <iostream>

int main() {

    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Test SFML");
    window.setFramerateLimit(60);

    //MENU
    Menu menu = Menu("assets/background/menu/bg_14.png");
    ControlManager ctrl(menu);

    Param param = Param(10);

    ctrl.bindAction(ControlManager::Action::JOUER,      [&]() { /* lancer la partie */ });
    ctrl.bindAction(ControlManager::Action::SCORES,     [&]() { /* afficher scores  */ });
    ctrl.bindAction(ControlManager::Action::PARAMETRES, [&]() { /* ouvrir settings  */ });
    ctrl.bindAction(ControlManager::Action::QUITTER,    [&]() { window.close();});
    
    while (window.isOpen()) {
    //evenements
        while (const std::optional event = window.pollEvent()) {
            ctrl.handleEvent(window, *event);
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        // Affichage
        window.clear(sf::Color::Black);    
        menu.display(window);
        //param.display(window); 
        window.display();
    }
}