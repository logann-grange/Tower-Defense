
#include "../score/Score.hpp"
#include <iostream>

// Constructeur par défaut
Score::Score() {
    playerName = "Unknown";
    points = 0;
    pv = 0;
    time = 0;
}

// Constructeur pratique (optionnel mais recommandé)
Score::Score(const string& name, int pts, int hp, time_t t) {
    playerName = name;
    points = pts;
    pv = hp;
    time = t;
}

void Score::print() const {
    std::cout << "Nom : " << playerName
              << " | Points : " << points
              << " | PV : " << pv
              << " | Temps : " << time
              << std::endl;
}
