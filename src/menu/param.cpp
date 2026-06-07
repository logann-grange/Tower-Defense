#include "../menu/param.hpp"

Param::Param(int sound) {

    this->sound = sound;
    // chargement des texture du background
    if (!bgTexture.loadFromFile("assets/UI/backgroundParam.png")) {
        std::cout << "ERREUR: texture de fond introuvable !" << std::endl;
    }
    bgSprite.emplace(bgTexture);
    bgSprite->setPosition({(1920-500)/2, 250});
    bgSprite->setScale({500.f/bgTexture.getSize().x, 720.f/bgTexture.getSize().y});

    //chargement des boutons
    buttonList.emplace_back(Vector2f(800, 550),  Vector2f(50, 50), "assets/UI/icone bouton/icone_plus.png",  "VOLUMEPLUS", false);
    buttonList.emplace_back(Vector2f(1065, 550),  Vector2f(50, 50), "assets/UI/icone bouton/icone_moins.png", "VOLUMEMOINS", false);
    buttonList.emplace_back(Vector2f(1150, 300),  Vector2f(50, 50), "assets/UI/icone bouton/icone_close.png", "RETOUR", false);
    buttonList.emplace_back(Vector2f((1920-400)/2, 750),  Vector2f(400,100), "assets/UI/icone bouton/bouton_bleu.png",  "CREDITS", true);

    //chargement de la police
    font = make_unique<Font>();
    font->openFromFile("assets/fonts/PressStart2P-Regular.ttf");

    //chargement des Labels
    soundLabel = make_unique<Text>(*font, "Volume du son", 24);
    soundLabel->setFillColor(Color(255, 174, 0));
    soundLabel->setPosition({800, 500});

    volumeLabel = make_unique<Text>(*font, to_string(sound), 24);
    volumeLabel->setFillColor(Color(255, 174, 0));
    volumeLabel->setPosition({935, 565});

    paramLabel = make_unique<Text>(*font, "PARAMETRES", 24);
    paramLabel->setFillColor(Color(255, 174, 0));
    paramLabel->setPosition({840, 350});
}

void Param::changeVolume(int volume) {
    this->sound += volume;
    this->sound = ((this->sound %11) + 11) % 11; // gère les limmite 
    volumeLabel = make_unique<Text>(*font, to_string(sound), 24);
    volumeLabel->setFillColor(Color(255, 174, 0));
    volumeLabel->setPosition({935, 565});
}

void Param::displaySound(RenderWindow &window){
    window.draw(*soundLabel);
    window.draw(*volumeLabel);
    window.draw(*paramLabel);

}

void Param::display(RenderWindow &window) {
    window.draw(*bgSprite);
    this->displaySound(window);
    //boutons
    int i = 0;
    for (auto& btn : this->buttonList) {
        btn.display(window);
    }
    
}

