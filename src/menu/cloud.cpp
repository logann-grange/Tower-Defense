#include "../menu/cloud.hpp"

Cloud::Cloud(string file, Vector2f coord, float speed) {

    this->coord = coord;
    this->speed = speed;
    // chargement des textures
    if (!Texture.loadFromFile(file)) {
        std::cout << "ERREUR: texture de fond introuvable !" << std::endl;
    }
    Sprite.emplace(Texture);

    //Sprite->setScale({600.f/titleTexture.getSize().x, 200.f/titleTexture.getSize().y});
    Sprite->setPosition(coord);
}

void Cloud::move() {
    if (this->coord.x > 1920-100) {
        this->coord.x = -500;
    } 
    this->coord.x += this->speed;
    Sprite->setPosition(this->coord);
}

void Cloud::display(RenderWindow &window) {
    window.draw(*Sprite);
}