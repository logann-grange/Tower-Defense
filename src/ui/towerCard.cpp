#include "../ui/towerCard.hpp"

TowerCard::TowerCard(const Tour& tower, const std::string& texturePath, Vector2f coord)
{
    this->tower = tower;

    // charger une texture persistante pour la carte de tour
    if (!towerTexture.loadFromFile(texturePath)) {
        cout << "ERREUR: texture de tour introuvable ! " << texturePath << std::endl;
    }
    towerSprite.emplace(towerTexture);
    towerSprite->setTexture(towerTexture, true);
    towerSprite->setPosition({coord.x + 50.f, coord.y + 65.f});
    towerSprite->setScale({100.f/bgTexture.getSize().x, 125.f/bgTexture.getSize().y});


    cout << "TowerCard créée pour : " << tower.getType() << endl;
    cout << "Sprite valide : " << (towerTexture.getSize().x != 0) << endl;
    // chargement des texture du background
    if (!bgTexture.loadFromFile("assets/UI/TowerCard.png")) {
        cout << "ERREUR: texture du fond du store introuvable !" << std::endl;
    }
    bgSprite.emplace(bgTexture);
    bgSprite->setPosition(coord);
    bgSprite->setScale({100.f/bgTexture.getSize().x, 125.f/bgTexture.getSize().y});

    //chargement des texture de l'indicateur de type
    if (!typeTexture.loadFromFile("assets/UI/icone tour/" + tower.getType() + ".png")) {
        cout << "ERREUR: texture du l'élément introuvable !" << std::endl;
    }
    typeSprite.emplace(typeTexture);
    typeSprite->setPosition({coord.x+4, coord.y+15});
    typeSprite->setScale({20.f/typeTexture.getSize().x, 20.f/typeTexture.getSize().y});

    //chargement de la police
    font = make_unique<Font>();
    if (!font->openFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "ERREUR: police introuvable !" << std::endl;
    }

    // chargement du label
    label = make_unique<Text>(*font, "Tour de " + tower.getType(), 6);
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
    if (towerSprite) window.draw(*towerSprite);
}

bool TowerCard::isClicked(sf::RenderWindow& window) {
    sf::Vector2i mousePos = sf::Mouse::getPosition(window);
    sf::Vector2f worldPos = window.mapPixelToCoords(mousePos);
    
    if (bgSprite.has_value()) {
        return bgSprite->getGlobalBounds().contains(worldPos);
    }
    return false;
}