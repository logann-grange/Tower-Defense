#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <optional>
#include <iostream>
#include "../tower/logic/Tour.hpp"

using namespace std;
using namespace sf;

class TowerCard {
    public :
        Tour tower;
        Texture towerTexture;
        optional<Sprite> towerSprite;
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
        TowerCard(const Tour& tower, const std::string& texturePath, Vector2f coord);

        void display(RenderWindow &window);
        bool isClicked(sf::RenderWindow& window);

};