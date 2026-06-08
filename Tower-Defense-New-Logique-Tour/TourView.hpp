#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <string>

/**
 * @class TourView
 * @brief Gère l'intégralité du rendu multimédia d'une tour (Sprites, textures et effets sonores).
 * Elle isole la bibliothèque SFML du reste de la simulation logique.
 */
class TourView
{
private:
    sf::Texture m_texture;
    sf::Sprite m_sprite;
    sf::SoundBuffer m_soundBuffer;
    sf::Sound m_sound;

public:
    // Le constructeur charge les images et prépare les composants audios
    TourView(sf::Vector2f position, const std::string& typeTour, const std::string& texturePath);
    
    void jouerSonTir();
    void draw(sf::RenderWindow &window) const; // Seule méthode autorisée à interagir avec l'écran
};
