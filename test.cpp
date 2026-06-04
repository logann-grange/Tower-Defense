#include <SFML/Graphics.hpp>
#include "bouton.hpp"
#include "menu.hpp"
#include "param.hpp"
#include "tabScores.hpp"
#include "controlManager.hpp"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Test SFML");
    window.setFramerateLimit(60);

    Menu menu("assets/background/menu/bg_14.png");
    Param param(10);

    tabScores score = tabScores();

    // On démarre avec les boutons du menu
    ControlManager ctrl(menu.buttonList);

    int state = 1;

    ctrl.bindAction(ControlManager::Action::JOUER,      [&]() { /* lancer la partie */ });
    ctrl.bindAction(ControlManager::Action::SCORES,     [&]() { state = 3; });
    ctrl.bindAction(ControlManager::Action::QUITTER,    [&]() { window.close(); });

    ctrl.bindAction(ControlManager::Action::PARAMETRES, [&]() {
        state = 4;
        ctrl.setActiveButtons(param.buttonList);  //switch vers parametres
    });

    ctrl.bindAction(ControlManager::Action::RETOUR, [&]() {
        state = 1;
        ctrl.setActiveButtons(menu.buttonList);   //retour vers menu
    });

    ctrl.bindAction(ControlManager::Action::VOLUMEPLUS,  [&]() { param.changeVolume(+1); });
    ctrl.bindAction(ControlManager::Action::VOLUMEMOINS, [&]() { param.changeVolume(-1); });

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            ctrl.handleEvent(window, *event);
            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        menu.display(window);
        if (state == 4) {
            param.display(window);
        }
        if (state == 3) {
            score.display(window);
        }
        
        window.display();
    }
}