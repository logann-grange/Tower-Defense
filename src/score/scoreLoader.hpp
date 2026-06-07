#pragma once
#include <iostream>
#include "../score/Score.hpp"

using namespace std;

class ScoreLoader {
    public :
        ScoreLoader();

        vector<Score> loadScoresFromJson(string file);
};