#pragma once
#include <iostream>
#include "bouton.hpp"
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

class Param {
    public :
        vector<Bouton> buttonList;
        Texture bgTexture;
        optional<Sprite> bgSprite;
        unique_ptr<Font> font; 
        unique_ptr<Text> soundLabel;
        unique_ptr<Text> volumeLabel;
        unique_ptr<Text> paramLabel;
        int sound;

        Param(int sound);

        void changeVolume(int volume);
        void displaySound(RenderWindow &window);
        void display(RenderWindow &window);
};