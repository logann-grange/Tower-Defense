#include "../include/TourView.hpp"
#include <iostream>

TourView::TourView(sf::Vector2f position, const std::string& typeTour, const std::string& texturePath)
    : m_sprite(m_texture),      // 🔥 CORRECTION SFML 3 : Initialisation du sprite liée à la texture
      m_sound(m_soundBuffer)    // Initialisation obligatoire du canal audio
{
    if (m_texture.loadFromFile(texturePath)) {
        m_sprite.setTexture(m_texture, true); 
        auto textureSize = m_texture.getSize();
        m_sprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(textureSize.x), static_cast<int>(textureSize.y)}));
        
        auto bounds = m_sprite.getLocalBounds();
        m_sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        m_sprite.setScale({0.08f, 0.08f});
        m_sprite.setPosition(position);
    } else {
        std::cerr << "[ERREUR VISUEL] Impossible de charger la texture : " << texturePath << "\n";
    }

    std::string soundPath = "asset/sound/tir_defaut.wav"; 
    if (typeTour == "Feu") soundPath = "asset/sound/tir_feu.wav";
    else if (typeTour == "Glace") soundPath = "asset/sound/tir_glace.wav";

    if (m_soundBuffer.loadFromFile(soundPath)) {
        m_sound.setBuffer(m_soundBuffer);
        m_sound.setVolume(50.f);
    } else {
        std::cerr << "[ERREUR AUDIO] Impossible de charger le son : " << soundPath << "\n";
    }
}

void TourView::jouerSonTir() {
    m_sound.play();
}

void TourView::draw(sf::RenderWindow &window) const {
    window.draw(m_sprite);
}