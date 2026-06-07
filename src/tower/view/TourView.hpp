#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "../logic/Tour.hpp"

class TourView {
public:
    TourView(const Tour& logique, const std::string& texturePath);

    void update(const Tour& logique);
    void draw(sf::RenderWindow& window) const;
    sf::Sprite getSpriteCopy() const;
    void playSound();

private:
    sf::Texture  m_texture;
    sf::Sprite   m_sprite;

    sf::SoundBuffer m_soundBuffer;
    sf::Sound       m_sound;

    void chargerSon(const std::string& type);
};
