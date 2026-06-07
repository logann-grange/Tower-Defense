#ifndef SCORE_HPP
#define SCORE_HPP

#include <iostream>
#include <string>
#include <ctime>

using namespace std;

class Score {
public:
    string playerName;
    int points;
    int pv;
    time_t time;

    Score();
    Score(const string& name, int pts, int hp, time_t t);

    void print() const;
};

#endif
