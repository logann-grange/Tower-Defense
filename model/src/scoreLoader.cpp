#include <iostream>
#include <vector>
#include "Score.hpp"
#include "scoreLoader.hpp"
#include <fstream>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

ScoreLoader::ScoreLoader() {}

std::vector<Score> ScoreLoader::loadScoresFromJson(string filename) {
    std::vector<Score> scores;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erreur : impossible d'ouvrir " << filename << std::endl;
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

    return scores;
}