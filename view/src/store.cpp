#include "store.hpp"

Store::Store(int gold) {
    // chargement des texture du background
    if (!bgTexture.loadFromFile("assets/UI/backgroundParam.png")) {
        cout << "ERREUR: texture du fond de la carte de tour introuvable !" << std::endl;
    }
    bgSprite.emplace(bgTexture);
    bgSprite->setPosition({687, -29});
    bgSprite->setScale({214.f/bgTexture.getSize().x, 584.f/bgTexture.getSize().y});

    //chargement des cartes
    //towerCardList.emplace_back(Tour(1, 0, 100, "feu", 0, 0, 1, {0, 0}, "assets/towers/fire/fire_tower_base.png"), Vector2f(650, 100));
    towerCardList.emplace_back(Tour(1, 0, 100, "glace", 0, 0, 1, {0, 0}, "assets/towers/fire/fire_tower_base.png"), Vector2f(650, 100));

}

void Store::display(RenderWindow &window) {
    window.draw(*bgSprite);

    for (auto& towerCard : this->towerCardList) {
        towerCard.display(window);
    }
}