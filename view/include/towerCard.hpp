#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <optional>
#include <iostream>
#include "Tour.hpp"

using namespace std;
using namespace sf;

class TowerCard {
    public :
        Tour tower;
        Texture bgTexture;
        optional<Sprite> bgSprite;
        Texture typeTexture;
        optional<Sprite> typeSprite;
        Vector2f coord;
        unique_ptr<Text> label;   
        unique_ptr<Text> price;
        unique_ptr<Font> font;

        TowerCard();
        //TowerCard(Tour tower, Vector2f coord);
        TowerCard(const Tour& tower, Vector2f coord);  

        void display(RenderWindow &window);
        bool isClicked(sf::RenderWindow& window);

};