#include "menu.hpp"
Menu::Menu(string background) {
    
    // chargement des texture du background
    if (!bgTexture.loadFromFile(background)) {
        std::cout << "ERREUR: texture de fond introuvable !" << std::endl;
    }
    bgSprite.emplace(bgTexture);

    // chargement des texture des arbres
    if (!bgTreeTexture.loadFromFile("assets/background/menu/arbres.png")) {
        std::cout << "ERREUR: texture des arbres introuvable !" << std::endl;
    }
    bgTreeSprite.emplace(bgTreeTexture);

    // chargement des texture du cadre
    if (!bgFrameTexture.loadFromFile("assets/background/menu/cadre.png")) {
        std::cout << "ERREUR: texture du cadre introuvable !" << std::endl;
    }
    bgFrameSprite.emplace(bgFrameTexture);

    // chargement des texture du titre
    if (!titleTexture.loadFromFile("assets/background/menu/titre.png")) {
        std::cout << "ERREUR: texture du cadre introuvable !" << std::endl;
    }
    titleSprite.emplace(titleTexture);

    float scaleX = 1920.f / bgTexture.getSize().x;
    float scaleY = 1080.f / bgTexture.getSize().y;
    bgSprite->setScale({scaleX, scaleY});
    bgTreeSprite->setScale({scaleX, scaleY});
    bgFrameSprite->setScale({scaleX, scaleY});
    titleSprite->setScale({600.f/titleTexture.getSize().x, 200.f/titleTexture.getSize().y});
    titleSprite->setPosition({(1920-600)/2, 50});

    //NUAGES
    this->cloud.push_back(make_unique<Cloud>("assets/background/menu/big_cloud.png", Vector2f(-100, -50), 0.5));
    this->cloud.push_back(make_unique<Cloud>("assets/background/menu/cloud.png", Vector2f(300, 100), 1));  

    buttonList.emplace_back(Vector2f((1920-400)/2, 300),  Vector2f(400,100), "assets/UI/icone bouton/bouton_bleu.png",  "JOUER", true);
    buttonList.emplace_back(Vector2f((1920-400)/2, 475),  Vector2f(400,100), "assets/UI/icone bouton/bouton_bleu.png",  "SCORES", true);
    buttonList.emplace_back(Vector2f((1920-400)/2, 650),  Vector2f(400,100), "assets/UI/icone bouton/bouton_bleu.png",  "PARAMETRES", true);
    buttonList.emplace_back(Vector2f((1920-400)/2, 825),  Vector2f(400,100), "assets/UI/icone bouton/bouton_rouge.png", "QUITTER", true);
}

void Menu::displayBackground(RenderWindow &window) {
    window.draw(*bgSprite);
    this->displayCloud(window);
    window.draw(*bgTreeSprite);
    window.draw(*bgFrameSprite);
    window.draw(*titleSprite);
}

void Menu::displayButton(RenderWindow &window) {
    int i = 0;
    for (auto& btn : this->buttonList) {
        btn.display(window);
    }
}

void Menu::display(RenderWindow &window) {
    this->displayBackground(window);
    this->displayButton(window);
    sf::RectangleShape overlay(sf::Vector2f(window.getSize()));
    overlay.setFillColor(sf::Color(0, 0, 0, 50)); // alpha : 0 = invisible, 255 = noir total
    window.draw(overlay);
}

void Menu::displayCloud(RenderWindow &window) {
    for (auto& cloud : this->cloud) {
        cloud->move();
        cloud->display(window);
    }
}