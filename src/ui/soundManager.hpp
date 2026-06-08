#pragma once
#include <SFML/Audio.hpp>
#include <iostream>
#include <string>
#include <optional>

using namespace std;

class SoundManager {
    public:
        sf::SoundBuffer m_buttonBuffer;
        std::optional<sf::Sound> m_buttonSound;

        sf::SoundBuffer m_projectileBuffer;
        std::optional<sf::Sound> m_projectileSound;
        sf::Music music;
        float currentVolume = 100.f;

        SoundManager();

        void playMusic(const string& gameState);
        void stopMusic();
        void playButtonSound(const string& buttonName);
        void setVolume(int volume);
        void playProjectileSound(const std::string& type, int niveau);
};