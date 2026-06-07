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
        float currentVolume = 100.f;  // volume courant

        SoundManager();

        void playMusic(const string& gameState);
        void stopMusic();
        void playButtonSound(const string& buttonName);
        void setVolume(int volume);
};