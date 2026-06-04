#pragma once
#include <iostream>
#include "Score.hpp"

using namespace std;

class ScoreLoader {
    public :
        ScoreLoader();

        vector<Score> loadScoresFromJson(string file);
};