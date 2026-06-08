#include "../ui/towerCard.hpp"

TowerCard::TowerCard(const Tour& tower, const std::string& texturePath, Vector2f coord)
{
    // chargement des texture du background
    if (!bgTexture.loadFromFile("assets/UI/TowerCard.png")) {
        cout << "ERREUR: texture du fond du store introuvable !" << std::endl;
    }
    bgSprite.emplace(bgTexture);
    bgSprite->setPosition(coord);
    bgSprite->setScale({180.f/bgTexture.getSize().x, 230.f/bgTexture.getSize().y});

    this->tower = tower;

    // chargement de la tour
    if (!towerTexture.loadFromFile(texturePath)) {
        cout << "ERREUR: texture de tour introuvable ! " << texturePath << std::endl;
    }
    towerSprite.emplace(towerTexture);
    towerSprite->setTexture(towerTexture, true);
    towerSprite->setPosition({coord.x + 5.f, coord.y + 5.f});
    towerSprite->setScale({180.f/towerTexture.getSize().x, 230.f/towerTexture.getSize().y});


    //chargement des texture de l'indicateur de type
    if (!typeTexture.loadFromFile("assets/UI/icone tour/" + tower.getType() + ".png")) {
        cout << "ERREUR: texture du l'élément introuvable !" << std::endl;
    }
    typeSprite.emplace(typeTexture);
    typeSprite->setPosition({coord.x+4, coord.y+30});
    typeSprite->setScale({40.f/typeTexture.getSize().x, 40.f/typeTexture.getSize().y});

    //chargement de la police
    font = make_unique<Font>();
    if (!font->openFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "ERREUR: police introuvable !" << std::endl;
    }

    // chargement du label
    label = make_unique<Text>(*font, "Tour de " + tower.getType(), 10);
    label->setFillColor(sf::Color(0, 0, 0));
    label->setPosition({coord.x+35, coord.y+30});
    // chargement du prix
    price = make_unique<Text>(*font, to_string(tower.getValeur()), 12);
    price->setFillColor(sf::Color(0, 0, 0));
    price->setPosition({coord.x+80, coord.y+185});

    cout << "bgSprite valide : " << bgSprite.has_value() << endl;
    cout << "typeSprite valide : " << typeSprite.has_value() << endl;

}

void TowerCard::display(RenderWindow &window) {
    if (bgSprite) window.draw(*bgSprite);
    if (typeSprite) window.draw(*typeSprite);
    if (label) window.draw(*label);
    if (price) window.draw(*price);
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