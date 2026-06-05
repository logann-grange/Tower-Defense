#include "towerCard.hpp"

TowerCard::TowerCard(Tour tower, Vector2f coord) {
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

}

void TowerCard::display(RenderWindow &window) {
    window.draw(*bgSprite);
    window.draw(*typeSprite);
    window.draw(*label);
    //window.draw(*tower.m_sprite);
}