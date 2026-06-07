#pragma once
#include <SFML/Graphics.hpp>
#include "scoreLoader.hpp"
#include "Score.hpp"
#include "bouton.hpp"

using namespace sf;
using namespace std;

class tabScores {
    public :
        vector<Score> scoreList;
        vector<Bouton> buttonList;
        vector<Text> labelList;
        unique_ptr<Font> font;
        unique_ptr<Text> title;
        Texture bgTexture;
        optional<Sprite> bgSprite;
    

        tabScores();
        void display(RenderWindow &window);

};