#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "bouton.hpp"
#include "menu.hpp"
#include "param.hpp"
#include "tabScores.hpp"
#include "controlManager.hpp"
#include "soundManager.hpp"
#include "Map.hpp"
#include "store.hpp"
#include "towerCard.hpp"
#include <iostream>

int main() {
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Test SFML");
    window.setFramerateLimit(60);

    Menu menu("assets/background/menu/bg_14.png");
    Param param(10);
    tabScores score = tabScores();
    SoundManager snd;
    Store store = Store(1000);

    ControlManager ctrl(menu.buttonList);
    int state = 1;

    snd.playMusic("menu");
    snd.setVolume(param.sound);

    ctrl.bindAction(ControlManager::Action::JOUER, [&]() {
        snd.playButtonSound("JOUER");
        /* lancer la partie */
    });
    ctrl.bindAction(ControlManager::Action::SCORES, [&]() {
        snd.playButtonSound("SCORES");
        state = 3;
        ctrl.setActiveButtons(score.buttonList);
    });
    ctrl.bindAction(ControlManager::Action::QUITTER, [&]() {
        snd.playButtonSound("QUITTER");
        window.close();
    });
    ctrl.bindAction(ControlManager::Action::PARAMETRES, [&]() {
        snd.playButtonSound("PARAMETRES");
        state = 4;
        ctrl.setActiveButtons(param.buttonList);
    });
    ctrl.bindAction(ControlManager::Action::RETOUR, [&]() {
        snd.playButtonSound("RETOUR");
        state = 1;
        ctrl.setActiveButtons(menu.buttonList);
    });
    ctrl.bindAction(ControlManager::Action::VOLUMEPLUS, [&]() {
        snd.playButtonSound("VOLUMEPLUS");
        param.changeVolume(+1);
        snd.setVolume(param.sound);
    });
    ctrl.bindAction(ControlManager::Action::VOLUMEMOINS, [&]() {
        snd.playButtonSound("VOLUMEMOINS");
        param.changeVolume(-1);
        snd.setVolume(param.sound);
    });

    Map map = Map();
    map.loadFromFile("assets/map/map_tower_defence.ldtk");

    View vueJeu(FloatRect({0.f, 0.f}, {900.f, 528.f}));
    window.setView(vueJeu);


    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            ctrl.handleEvent(window, *event);

        // clic sur une towerCard
            if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (mouseEvent->button == sf::Mouse::Button::Left) {
                    for (auto& card : store.towerCardList) {
                        if (card->isClicked(window)) {
                            store.selectedTower = card->tower;
                            cout << "Tour sélectionnée : " << card->tower.m_type << endl;
                        }
                    }
                }
            }

            if (event->is<sf::Event::Closed>())
                window.close();
        }

        window.clear(sf::Color::Black);
        menu.display(window);
        if (state == 4) param.display(window);
        if (state == 3) score.display(window);
        map.draw(window);
        store.display(window);
        window.display();
    }
}
