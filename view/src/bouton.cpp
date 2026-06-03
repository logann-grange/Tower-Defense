#include "./../include/bouton.hpp"

Bouton::Bouton(sf::Vector2f coord, sf::Vector2f size, std::string image, std::string text)
    : coord(coord), size(size), image(image), text(text), box(size)
{
    box.setPosition(coord);
    texture.loadFromFile(image);

    // unique_ptr : adresse fixe même si le vecteur se redimensionne
    font = std::make_unique<sf::Font>();
    font->openFromFile("assets/fonts/PressStart2P-Regular.ttf");

    label = std::make_unique<sf::Text>(*font, text, 24);
    label->setFillColor(sf::Color(255, 174, 0));

    sf::FloatRect bounds = label->getLocalBounds();
    label->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    label->setPosition({coord.x + size.x / 2.f, coord.y + size.y / 2.f});
}

bool Bouton::isHover(sf::RenderWindow &window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Sprite sprite(texture);
    sprite.setPosition(coord);
    sf::Vector2u texSize = texture.getSize();
    sprite.setScale({size.x / texSize.x, size.y / texSize.y});
    return sprite.getGlobalBounds().contains(sf::Vector2f(mousePos));
}

void Bouton::display(sf::RenderWindow &window) {
    sf::Sprite sprite(texture);
    sprite.setPosition(coord);
    sf::Vector2u texSize = texture.getSize();
    sprite.setScale({size.x / texSize.x, size.y / texSize.y});

    if (isHover(window) && this->text == "QUITTER") {
        sprite.setColor(sf::Color(255, 100, 100));
        label->setFillColor(sf::Color::Yellow);
    } else if (isHover(window)) {
        sprite.setColor(sf::Color(255, 255, 150));
        label->setFillColor(sf::Color::Yellow);
    } else {
        sprite.setColor(sf::Color::White);
        label->setFillColor(sf::Color(255, 174, 0));
    }

    window.draw(sprite);
    window.draw(*label);
}

bool Bouton::clicOn(sf::RenderWindow &window) {
    return isHover(window) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left);
}