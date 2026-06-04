#include "tabScores.hpp"

tabScores::tabScores() {

    ScoreLoader scorelaoder = ScoreLoader();

    scoreList = scorelaoder.loadScoresFromJson("../../data/scores.json");

    // chargement des texture du background
    if (!bgTexture.loadFromFile("./assets/UI/backgroundScore.png")) {
        std::cout << "ERREUR: texture de fond introuvable !" << std::endl;
    }
    bgSprite.emplace(bgTexture);

    // chargement de la police
    font = make_unique<Font>();
    font->openFromFile("assets/fonts/PressStart2P-Regular.ttf");

    // initialisation du titre
    title = make_unique<Text>(*font, "SCORES", 48);
    title->setFillColor(sf::Color(255, 174, 0));
    title->setPosition({800, 400});

    // chargement des labels
    float y = 500;
    for (auto& i : scoreList) {
        sf::Text label(*font, i.playerName + " : " + to_string(i.points) + " " + to_string(i.time) + " " + to_string(i.pv), 24);
        label.setFillColor(sf::Color(255, 174, 0));
        label.setPosition({800, y});
        y += 40;
        labelList.push_back(label);
    }
}

void tabScores::display(RenderWindow &window) {
    window.draw(*bgSprite);
    window.draw(*title);
    for (auto& label : labelList) {
        window.draw(label);
    }
}