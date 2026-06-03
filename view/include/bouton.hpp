#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <memory>
#include <optional>

class Bouton {
public:
    std::string text;
    Bouton(sf::Vector2f coord, sf::Vector2f size, std::string image, std::string text);

    void display(sf::RenderWindow &window);
    bool isHover(sf::RenderWindow &window);
    bool clicOn(sf::RenderWindow &window);

private:
    sf::Vector2f coord;
    sf::Vector2f size;
    std::string image;

    sf::RectangleShape box;
    sf::Texture texture;

    std::unique_ptr<sf::Font> font; 
    std::unique_ptr<sf::Text> label;
};