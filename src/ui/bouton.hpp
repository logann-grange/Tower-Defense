#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <optional>
#include <iostream>

using namespace std;
using namespace sf;

class Bouton {
public:
    string text;
    string action;
    bool isLabel;
    Bouton(Vector2f coord, Vector2f size, string image, string text, bool isLabel);

    void display(RenderWindow &window);
    bool isHover(RenderWindow &window);
    bool clicOn(RenderWindow &window);

private:
    Vector2f coord;
    Vector2f size;
    string image;

    RectangleShape box;
    Texture texture;

    unique_ptr<Font> font; 
    unique_ptr<Text> label;
};