#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include "bouton.hpp"
#include "Tour.hpp"
#include "towerCard.hpp"

using namespace std;
using namespace sf;

class Store {
    public :
        vector<Bouton> buttonList;
        Texture bgTexture;
        optional<Sprite> bgSprite;
        vector<Tour> towerList; // à supprimer ??
        vector<TowerCard> towerCardList; // fiche d'achat des tours (prix, nom, sprite...)
        Tour selectedTower;
        int gold;

        Store(int gold);

        void display(RenderWindow &window);


};