#pragma once
#include <SFML/Graphics.hpp>
#include "scoreLoader.hpp"
#include "Score.hpp"

using namespace sf;

class tabScores {
    public :
        vector<Score> scoreList;
        vector<Text> labelList;
        unique_ptr<Font> font;
        unique_ptr<Text> title;
        Texture bgTexture;
        optional<Sprite> bgSprite;
    

        tabScores();
        void display(RenderWindow &window);

};