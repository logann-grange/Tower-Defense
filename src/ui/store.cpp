#include "../ui/store.hpp"

Store::Store(int gold) {
    // chargement des texture du background
    if (!bgTexture.loadFromFile("assets/UI/backgroundParam.png")) {
        cout << "ERREUR: texture du fond de la carte de tour introuvable !" << std::endl;
    }
    bgSprite.emplace(bgTexture);
    bgSprite->setPosition({1376, -50});
    bgSprite->setScale({200.f/bgTexture.getSize().x, 1156.f/bgTexture.getSize().y});

    //chargement des cartes
    Tour tourFeu(1, 15, 60, "feu", 8, 1.5f, 1, {0,0});
    Tour tourGlace(1, 10, 70, "glace", 6, 2.0f, 1, {0,0});

    // create views for the tower cards

    towerCardList.push_back(make_unique<TowerCard>(tourFeu, "assets/towers/fire/fire_tower_base.png", Vector2f(1385, 150)));
    towerCardList.push_back(make_unique<TowerCard>(tourGlace, "assets/towers/ice/ice_tower_base.png", Vector2f(1385, 400)));

    //chargement de la police
    font = make_unique<Font>();
    if (!font->openFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "ERREUR: police introuvable !" << std::endl;
    }

    //chargement de l'indicateur d'or
    this->gold = gold;
    goldLabel = make_unique<Text>(*font, to_string(gold), 18);
    goldLabel->setFillColor(Color(255, 174, 0));
    goldLabel->setPosition({1460, 69});

    //chagement de l'icone d'or
    if (!goldTexture.loadFromFile("assets/UI/icone bouton/gold.png")) {
        cout << "ERREUR: texture de l'icone gold introuvable !" << std::endl;
    }
    goldSprite.emplace(goldTexture);
    goldSprite->setPosition({1386, 50});
    goldSprite->setScale({60.f/goldTexture.getSize().x, 60.f/goldTexture.getSize().y});


}

void Store::display(RenderWindow &window) {
    window.draw(*bgSprite);
    window.draw(*goldLabel);
    window.draw(*goldSprite);

    for (auto& towerCard : this->towerCardList) {
        towerCard->display(window);
    }
}