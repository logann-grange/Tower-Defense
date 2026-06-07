#include "../ui/store.hpp"

Store::Store(int gold) {
    // chargement des texture du background
    if (!bgTexture.loadFromFile("assets/UI/backgroundParam.png")) {
        cout << "ERREUR: texture du fond de la carte de tour introuvable !" << std::endl;
    }
    bgSprite.emplace(bgTexture);
    bgSprite->setPosition({1376, 0});
    bgSprite->setScale({200.f/bgTexture.getSize().x, 1056.f/bgTexture.getSize().y});

    //chargement des cartes
    Tour tourFeu(1, 15, 100, "feu", 8, 1.5f, 1, {0,0});
    Tour tourGlace(1, 10, 100, "glace", 6, 2.0f, 1, {0,0});

    // create views for the tower cards

    towerCardList.push_back(make_unique<TowerCard>(tourFeu,   "assets/towers/fire/fire_tower_base.png",  Vector2f(1385, 100)));
towerCardList.push_back(make_unique<TowerCard>(tourGlace, "assets/towers/ice/ice_tower_base.png",   Vector2f(1490, 100)));

    //chargement de la police
    font = make_unique<Font>();
    if (!font->openFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "ERREUR: police introuvable !" << std::endl;
    }

    //chargement de l'indicateur d'or
    this->gold = gold;
    goldLabel = make_unique<Text>(*font, to_string(gold), 10);
    goldLabel->setFillColor(Color(255, 174, 0));
    goldLabel->setPosition({1420, 23});

    //chagement de l'icone d'or
    if (!goldTexture.loadFromFile("assets/UI/icone bouton/gold.png")) {
        cout << "ERREUR: texture de l'icone gold introuvable !" << std::endl;
    }
    goldSprite.emplace(goldTexture);
    goldSprite->setPosition({1386, 10});
    goldSprite->setScale({30.f/goldTexture.getSize().x, 30.f/goldTexture.getSize().y});


}

void Store::display(RenderWindow &window) {
    window.draw(*bgSprite);
    window.draw(*goldLabel);
    window.draw(*goldSprite);

    for (auto& towerCard : this->towerCardList) {
        towerCard->display(window);
    }
}