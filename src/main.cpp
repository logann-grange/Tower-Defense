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
    sf::RenderWindow window(sf::VideoMode({1920, 1080}), "Tower Defence");
    window.setFramerateLimit(60);
 
    // --- ÉTAT DU JEU ---
    GameState state = GameState::Menu;
 
    // --- MENU ---
    Menu menu("assets/background/menu/bg_14.png");
    Param param(10);
    tabScores score;
    SoundManager snd;
    Store store(1000);
    ControlManager ctrl(menu.buttonList);
 
    snd.playMusic("menu");
    snd.setVolume(param.sound);
 
    // --- CARTE ET JEU ---
    Map mon_niveau;
    if (!mon_niveau.loadFromFile("assets/map/map_tower_defence.ldtk")) {
        std::cerr << "Erreur : Impossible de charger la carte !" << std::endl;
        return -1;
    }
 
    sf::View vueMenu(sf::FloatRect({0.f, 0.f}, {1920.f, 1080.f}));
    sf::View vueJeu(sf::FloatRect({0.f, 0.f}, {900.f, 528.f}));
 
    GameManager gameManager(mon_niveau);
    if (!gameManager.initialiser("./data/Vague.json")) {
        return -1;
    }
 
    sf::Clock clock;
 
    // --- ACTIONS DU MENU ---
    ctrl.bindAction(ControlManager::Action::JOUER, [&]() {
        snd.playButtonSound("JOUER");
        state = GameState::InGame;
        snd.playMusic("game");
        window.setView(vueJeu);
    });
    ctrl.bindAction(ControlManager::Action::SCORES, [&]() {
        snd.playButtonSound("SCORES");
        state = GameState::Scores;
        ctrl.setActiveButtons(score.buttonList);
    });
    ctrl.bindAction(ControlManager::Action::QUITTER, [&]() {
        snd.playButtonSound("QUITTER");
        window.close();
    });
    ctrl.bindAction(ControlManager::Action::PARAMETRES, [&]() {
        snd.playButtonSound("PARAMETRES");
        state = GameState::Parametres;
        ctrl.setActiveButtons(param.buttonList);
    });
    ctrl.bindAction(ControlManager::Action::RETOUR, [&]() {
        snd.playButtonSound("RETOUR");
        state = GameState::Menu;
        ctrl.setActiveButtons(menu.buttonList);
        window.setView(vueMenu);
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
 
    // --- BOUCLE PRINCIPALE ---
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
 
        // --- EVENTS ---
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
 
            if (state == GameState::Menu ||
                state == GameState::Scores ||
                state == GameState::Parametres) {
                ctrl.handleEvent(window, *event);
            }
 
            if (state == GameState::InGame) {
                if (const auto* mouseEvent = event->getIf<sf::Event::MouseButtonPressed>()) {
                    if (mouseEvent->button == sf::Mouse::Button::Left) {
                        for (auto& card : store.towerCardList) {
                            if (card->isClicked(window)) {
                                store.selectedTower = card->tower;
                                std::cout << "Tour selectionnee : " << card->tower.m_type << std::endl;
                            }
                        }
                    }
                }
            }
        }
 
        // --- UPDATE ---
        if (state == GameState::InGame) {
            if (!gameManager.isGameOver()) {
                gameManager.update(deltaTime);
            }
            if (gameManager.isGameOver()) {
                state = GameState::GameOver;
                snd.playMusic("gameover");
            }
            if (gameManager.isVictoire()) {
                state = GameState::Victory;
                snd.playMusic("victoire");
            }
        }
 
        // --- RENDU ---
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
            store.display(window);
        }
 
        if (state == GameState::GameOver) {
            window.setView(vueMenu);
            // TODO : afficher ecran Game Over
        }
 
        if (state == GameState::Victory) {
            window.setView(vueMenu);
            // TODO : afficher ecran Victory
        }
 
        window.display();
    }
 
    return 0;
}