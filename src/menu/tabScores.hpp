#pragma once
#include <SFML/Graphics.hpp>
#include "../score/scoreLoader.hpp"
#include "../score/Score.hpp"
#include "../ui/bouton.hpp"

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