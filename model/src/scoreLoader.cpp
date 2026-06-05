#include <iostream>
#include <vector>
#include "Score.hpp"
#include "scoreLoader.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ScoreLoader::ScoreLoader() {}

vector<Score> ScoreLoader::loadScoresFromJson(string filename) {
    vector<Score> scores;

    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Erreur : impossible d'ouvrir " << filename << endl;
        return scores;
    }

    json data;
    file >> data;

    for (auto& entry : data["scores"]) {
        std::string name = entry["name"];
        int points = entry["points"];
        int pv = entry["pv"];
        int temps = entry["temps"];

        time_t t = static_cast<time_t>(temps);

        scores.emplace_back(name, points, pv, t);
    }

    // Trier par points décroissant
    sort(scores.begin(), scores.end(), [](const Score &a, const Score &b) {
        return a.points > b.points;
    });

    // Garder seulement les 10 meilleurs
    if (scores.size() > 10)
        scores.resize(10);

    return scores;
}