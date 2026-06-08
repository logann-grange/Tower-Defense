#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

class TourView
{
private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::SoundBuffer m_soundBuffer;
    sf::Sound m_sound;

public:
    TourView(sf::Vector2f position, const std::string& typeTour, const std::string& texturePath);
    
    void jouerSonTir();
    void draw(sf::RenderWindow &window) const;
};