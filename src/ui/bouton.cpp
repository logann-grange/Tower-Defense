#include "../ui/bouton.hpp"

Bouton::Bouton(Vector2f coord, Vector2f size, string image, string text, bool isLabel)
    : coord(coord), size(size), image(image), text(text), box(size), isLabel(isLabel)
{
    box.setPosition(coord);
    if (!texture.loadFromFile(image)) {
        std::cerr << "ERREUR: impossible de charger la texture: " << image << std::endl;
    }

    // unique_ptr : adresse fixe même si le vecteur se redimensionne
    font = make_unique<Font>();
    if (!font->openFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "ERREUR: police introuvable !" << std::endl;
    }

    label = make_unique<Text>(*font, text, 24);
    label->setFillColor(Color(255, 174, 0));

    FloatRect bounds = label->getLocalBounds();
    label->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
    label->setPosition({coord.x + size.x / 2.f, coord.y + size.y / 2.f});
}

bool Bouton::isHover(RenderWindow &window) {
    Vector2i mousePos = Mouse::getPosition(window);
    Vector2f worldPos = window.mapPixelToCoords(mousePos);
    Sprite sprite(texture);
    sprite.setPosition(coord);
    Vector2u texSize = texture.getSize();
    sprite.setScale({size.x / texSize.x, size.y / texSize.y});
    return sprite.getGlobalBounds().contains(worldPos);
}

void Bouton::display(RenderWindow &window) {
    Sprite sprite(texture);
    sprite.setPosition(coord);
    Vector2u texSize = texture.getSize();
    sprite.setScale({size.x / texSize.x, size.y / texSize.y});

    if (isHover(window) && (this->text == "QUITTER" || this->text == "RETOUR")) {
        sprite.setColor(sf::Color(255, 100, 100));
        label->setFillColor(sf::Color::Yellow);
    } else if (isHover(window)) {
        sprite.setColor(Color(255, 255, 150));
        label->setFillColor(Color::Yellow);
    } else {
        sprite.setColor(Color::White);
        label->setFillColor(Color(255, 174, 0));
    }

    window.draw(sprite);
    if(this->isLabel == true) {
        window.draw(*label);
    }
    
}

bool Bouton::clicOn(RenderWindow &window) {
    return isHover(window) && Mouse::isButtonPressed(Mouse::Button::Left);
}