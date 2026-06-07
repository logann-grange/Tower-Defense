#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../ui/bouton.hpp"
#include "../tower/logic/Tour.hpp"
#include "../tower/logic/Tour.hpp"
#include "../tower/view/TourView.hpp"
#include "../ui/towerCard.hpp"

using namespace std;
using namespace sf;

class Store {
    public :
        vector<Bouton> buttonList;
        Texture bgTexture;
        optional<Sprite> bgSprite;
        vector<Tour> towerList; // à supprimer ??
        vector<unique_ptr<TowerCard>> towerCardList;
        unique_ptr<Text> goldLabel;
        unique_ptr<Font> font;
        Texture goldTexture;
        optional<Sprite> goldSprite;
        Tour selectedTower;
        int gold;

        Store(int gold);
        void updateGold(int newGold) {
        gold = newGold;
        goldLabel->setString(std::to_string(gold));
}

        void display(RenderWindow &window);


};