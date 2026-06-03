#pragma once
#include <iostream>
#include <optional>
#include "bouton.hpp"
#include "cloud.hpp"

using namespace std;
using namespace sf;

class Menu {
    public :
        string background;
        vector<Bouton> buttonList;
        vector<Texture> cloudList; 
        Texture bgTexture;
        optional<Sprite> bgSprite;  // optional car probleme de constructeur par défaut en SFML 3
        Texture bgTreeTexture;
        optional<Sprite> bgTreeSprite;
        Texture bgFrameTexture;
        optional<Sprite> bgFrameSprite;
        Texture titleTexture;
        optional<Sprite> titleSprite;
        vector<unique_ptr<Cloud>> cloud;

        Menu(string background);

        void display(RenderWindow &window);
        void displayButton(RenderWindow &window);
        void displayBackground(RenderWindow &window);
        void displayCloud(RenderWindow &window);
};