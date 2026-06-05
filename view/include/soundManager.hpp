#pragma once
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <optional>

using namespace std;

class SoundManager {
    public:
        sf::SoundBuffer buffer;
        std::optional<sf::Sound> sound;
        sf::Music music;

        SoundManager();

        void playMusic(const string& gameState);
        void stopMusic();
        void playButtonSound(const string& buttonName);
};