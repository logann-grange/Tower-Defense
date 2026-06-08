#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>

#include "GameState.hpp"
#include "Map/Map.hpp"
#include "gameManager/GameManager.hpp"
#include "menu/menu.hpp"
#include "menu/param.hpp"
#include "menu/tabScores.hpp"
#include "ui/store.hpp"
#include "ui/soundManager.hpp"
#include "controller/controlManager.hpp"

int main() {
    sf::RenderWindow window(sf::VideoMode({1576, 1056}), "Tower Defence");
    sf::View vueJeu(sf::FloatRect({0.f, 0.f}, {688.f, 528.f}));
    vueJeu.setViewport(sf::FloatRect({0.f, 0.f}, {1376.f/1576.f, 1.f}));
    sf::View vueUI(sf::FloatRect({0.f, 0.f}, {1576.f, 1056.f}));
    sf::View vueMenu(sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}));
    window.setFramerateLimit(60);

    GameState state = GameState::Menu;

    std::cout << "1 - Menu..." << std::endl;
    Menu menu("assets/background/menu/bg_14.png");
    std::cout << "2 - Param..." << std::endl;
    Param param(10);
    std::cout << "3 - tabScores..." << std::endl;
    tabScores score;
    std::cout << "4 - SoundManager..." << std::endl;
    SoundManager snd;
    std::cout << "5 - Store..." << std::endl;
    Store store(1000);
    std::cout << "6 - ControlManager..." << std::endl;
    ControlManager ctrl(menu.buttonList);

    std::cout << "7 - playMusic..." << std::endl;
    snd.playMusic("menu");
    snd.setVolume(param.sound);

    std::cout << "8 - Map..." << std::endl;
    Map mon_niveau;
    if (!mon_niveau.loadFromFile("assets/map/map_tower_defence.ldtk")) {
        std::cerr << "Erreur : Impossible de charger la carte !" << std::endl;
        return -1;
    }

    std::cout << "9 - GameManager..." << std::endl;
    GameManager gameManager(mon_niveau);
    if (!gameManager.initialiser("./data/Vague.json")) {
        return -1;
    }

    std::cout << "10 - Clock..." << std::endl;
    sf::Clock clock;

    std::cout << "11 - bindAction JOUER..." << std::endl;
    ctrl.bindAction(ControlManager::Action::JOUER, [&]() {
        snd.playButtonSound("JOUER");
        state = GameState::InGame;
        // snd.playMusic("game");
    });
    std::cout << "12 - bindAction SCORES..." << std::endl;
    ctrl.bindAction(ControlManager::Action::SCORES, [&]() {
        snd.playButtonSound("SCORES");
        state = GameState::Scores;
        ctrl.setActiveButtons(score.buttonList);
    });
    std::cout << "13 - bindAction QUITTER..." << std::endl;
    ctrl.bindAction(ControlManager::Action::QUITTER, [&]() {
        snd.playButtonSound("QUITTER");
        window.close();
    });
    std::cout << "14 - bindAction PARAMETRES..." << std::endl;
    ctrl.bindAction(ControlManager::Action::PARAMETRES, [&]() {
        snd.playButtonSound("PARAMETRES");
        state = GameState::Parametres;
        ctrl.setActiveButtons(param.buttonList);
    });
    std::cout << "15 - bindAction RETOUR..." << std::endl;
    ctrl.bindAction(ControlManager::Action::RETOUR, [&]() {
        snd.playButtonSound("RETOUR");
        state = GameState::Menu;
        ctrl.setActiveButtons(menu.buttonList);
    });
    std::cout << "16 - bindAction VOLUMEPLUS..." << std::endl;
    ctrl.bindAction(ControlManager::Action::VOLUMEPLUS, [&]() {
        snd.playButtonSound("VOLUMEPLUS");
        param.changeVolume(+1);
        snd.setVolume(param.sound);
    });
    std::cout << "17 - bindAction VOLUMEMOINS..." << std::endl;
    ctrl.bindAction(ControlManager::Action::VOLUMEMOINS, [&]() {
        snd.playButtonSound("VOLUMEMOINS");
        param.changeVolume(-1);
        snd.setVolume(param.sound);
    });
    std::cout << "18 - Boucle principale..." << std::endl;

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();

            if (state == GameState::Menu ||
                state == GameState::Scores ||
                state == GameState::Parametres) {
                window.setView(vueMenu);
                ctrl.handleEvent(window, *event);
            }

            if (state == GameState::InGame) {
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {

                        window.setView(vueUI);
                        for (auto& card : store.towerCardList) {
                            if (card->isClicked(window)) {
                                store.selectedTower = card->tower;
                                std::cout << "Tour selectionnee : " << store.selectedTower.getType() << std::endl;
                            }
                        }

                        if (store.selectedTower.getType() != "") {
                            sf::Vector2f posMonde = window.mapPixelToCoords(
                                mouseEvent->position, vueJeu);

                            if (posMonde.x >= 0 && posMonde.x < 688.f &&
                                posMonde.y >= 0 && posMonde.y < 528.f) {

                                sf::Vector2f posGrille = {
                                    std::floor(posMonde.x / 16.f) * 16.f + 8.f,
                                    std::floor(posMonde.y / 16.f) * 16.f + 8.f
                                };

                                Tour tourAPoser(
                                    store.selectedTower.getId(),
                                    store.selectedTower.getAtk(),
                                    store.selectedTower.getValeur(),
                                    store.selectedTower.getType(),
                                    store.selectedTower.getPortee(),
                                    store.selectedTower.getVitesse(),
                                    store.selectedTower.getNiveau(),
                                    posGrille
                                );

                                std::string texturePath = "assets/towers/fire/fire_tower_base.png";
                                if (store.selectedTower.getType() == "glace")
                                    texturePath = "assets/towers/ice/ice_tower_base.png";

                                gameManager.placerTour(tourAPoser, texturePath);
                                store.updateGold(gameManager.getOr());
                                store.selectedTower = Tour();
                            }
                        }
                    }
                }
            }
        }

        if (state == GameState::InGame) {
            if (!gameManager.isGameOver()) {
                gameManager.update(deltaTime);
                store.gold = gameManager.getOr();
            }
            if (gameManager.isGameOver()) {
                state = GameState::GameOver;
            }
            if (gameManager.isVictoire()) {
                state = GameState::Victory;
            }
        }

        window.clear(sf::Color::Black);

        if (state == GameState::Menu ||
            state == GameState::Scores ||
            state == GameState::Parametres) {
            window.setView(vueMenu);
            menu.display(window);
            if (state == GameState::Parametres) param.display(window);
            if (state == GameState::Scores)     score.display(window);
        }

        if (state == GameState::InGame) {
            window.setView(vueJeu);
            mon_niveau.draw(window);
            gameManager.draw(window);
            window.setView(vueUI);
            store.display(window);
        }

        if (state == GameState::GameOver) {
            window.setView(vueMenu);
            state = GameState::Menu;
        }

        if (state == GameState::Victory) {
            window.setView(vueMenu);
            state = GameState::Menu;
        }

        window.display();
    }

    return 0;
}