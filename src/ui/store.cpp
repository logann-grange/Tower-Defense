#include "../ui/store.hpp"

Store::Store(int gold) {
    // chargement des texture du background
    if (!bgTexture.loadFromFile("assets/UI/backgroundParam.png")) {
        cout << "ERREUR: texture du fond de la carte de tour introuvable !" << std::endl;
    }
    bgSprite.emplace(bgTexture);
    bgSprite->setPosition({687, -29});
    bgSprite->setScale({214.f/bgTexture.getSize().x, 584.f/bgTexture.getSize().y});

    //chargement des cartes
    Tour tourFeu(1, 0, 100, "feu", 0, 0, 1, {0,0}, "assets/towers/fire/fire_tower_base.png");
    Tour tourGlace(1, 0, 100, "glace", 0, 0, 1, {0,0}, "assets/towers/ice/ice_tower_base.png");

    towerCardList.push_back(make_unique<TowerCard>(tourFeu,   Vector2f(690, 100)));
    towerCardList.push_back(make_unique<TowerCard>(tourGlace, Vector2f(795, 100)));

    //chargement de la police
    font = make_unique<Font>();
    font->openFromFile("assets/fonts/PressStart2P-Regular.ttf");

    //chargement de l'indicateur d'or
    this->gold = gold;
    goldLabel = make_unique<Text>(*font, to_string(gold), 10);
    goldLabel->setFillColor(Color(255, 174, 0));
    goldLabel->setPosition({730, 23});

    //chagement de l'icone d'or
    if (!goldTexture.loadFromFile("assets/UI/icone bouton/gold.png")) {
        cout << "ERREUR: texture de l'icone gold introuvable !" << std::endl;
    }
    goldSprite.emplace(goldTexture);
    goldSprite->setPosition({700, 10});
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