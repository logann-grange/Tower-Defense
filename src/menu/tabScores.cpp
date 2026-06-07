#include "../menu/tabScores.hpp"

tabScores::tabScores() {

    ScoreLoader scorelaoder = ScoreLoader();

    scoreList = scorelaoder.loadScoresFromJson("data/scores.json");

    // chargement des texture du background
    if (!bgTexture.loadFromFile("./assets/UI/backgroundScore.png")) {
        std::cout << "ERREUR: texture de fond introuvable !" << std::endl;
    }
    bgSprite.emplace(bgTexture);
    bgSprite->setPosition({(1920-1144)/2, (1080-612)/2});

    // chargement de la police
    font = make_unique<Font>();
    if (!font->openFromFile("assets/fonts/PressStart2P-Regular.ttf")) {
        std::cerr << "ERREUR: police introuvable !" << std::endl;
    }

    // initialisation du titre
    title = make_unique<Text>(*font, "SCORES", 32);
    title->setFillColor(sf::Color(106, 123, 128));
    title->setPosition({850, 320});

    // chargement des labels
    float y = 365;
    auto end = scoreList.begin() + min(10, (int)scoreList.size());
    for (auto it = scoreList.begin(); it != end; ++it) {
        auto& i = *it;
        sf::Text label(*font, i.playerName + " : " + to_string(i.points) + "pts " + to_string(i.time) + " " + to_string(i.pv) + "pv", 24);
        label.setFillColor(sf::Color(20, 20, 20));
        label.setPosition({600, y});
        y += 40;
        labelList.push_back(label);
    }

    //BOUTON
    buttonList.emplace_back(Vector2f(1300, 300),  Vector2f(50, 50), "assets/UI/icone bouton/icone_close.png", "RETOUR", false);
}

void tabScores::display(RenderWindow &window) {
    window.draw(*bgSprite);
    window.draw(*title);
    for (auto& label : labelList) {
        window.draw(label);
    }

    //boutons
    int i = 0;
    for (auto& btn : this->buttonList) {
        btn.display(window);
    }
}