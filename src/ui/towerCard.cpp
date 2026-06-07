#include "../ui/towerCard.hpp"

TowerCard::TowerCard(const Tour& tower, Vector2f coord) {
    //modif des coord de la tour
    this->tower = tower;
    if (this->tower.m_sprite.has_value()) {
        this->tower.m_sprite->setPosition({coord.x + 50.f, coord.y + 65.f});
    }
    cout << "TowerCard créée pour : " << tower.m_type << endl;
    cout << "Sprite valide : " << tower.m_sprite.has_value() << endl;
    // chargement des texture du background
    if (!bgTexture.loadFromFile("assets/UI/TowerCard.png")) {
        cout << "ERREUR: texture du fond du store introuvable !" << std::endl;
    }
    bgSprite.emplace(bgTexture);
    bgSprite->setPosition(coord);
    bgSprite->setScale({100.f/bgTexture.getSize().x, 125.f/bgTexture.getSize().y});

    //chargement des texture de l'indicateur de type
    if (!typeTexture.loadFromFile("assets/UI/icone tour/" + tower.m_type + ".png")) {
        cout << "ERREUR: texture du l'élément introuvable !" << std::endl;
    }
    typeSprite.emplace(typeTexture);
    typeSprite->setPosition({coord.x+4, coord.y+15});
    typeSprite->setScale({20.f/typeTexture.getSize().x, 20.f/typeTexture.getSize().y});

    //chargement de la police
    font = make_unique<Font>();
    font->openFromFile("assets/fonts/PressStart2P-Regular.ttf");

    // chargement du label
    label = make_unique<Text>(*font, "Tour de " + tower.m_type, 6);
    label->setFillColor(sf::Color(0, 0, 0));
    label->setPosition({coord.x+20, coord.y+15});
    // a enlever ??

    cout << "bgSprite valide : " << bgSprite.has_value() << endl;
    cout << "typeSprite valide : " << typeSprite.has_value() << endl;

}

void TowerCard::display(RenderWindow &window) {
    if (bgSprite) window.draw(*bgSprite);
    if (typeSprite) window.draw(*typeSprite);
    if (label) window.draw(*label);
    if (tower.m_sprite) window.draw(*tower.m_sprite);
}

bool TowerCard::isClicked(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    
    if (bgSprite.has_value()) {
        return bgSprite->getGlobalBounds().contains(worldPos);
    }
    return false;
}