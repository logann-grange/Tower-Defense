#include "../include/TourView.hpp"
#include <iostream>

TourView::TourView(sf::Vector2f position, const std::string& typeTour, const std::string& texturePath)
    : m_sprite(m_texture),      // 👑 SÉCURITÉ SFML 3 : Liaison explicite obligatoire du Sprite avec sa Texture dès l'initialisation
      m_sound(m_soundBuffer)    // 👑 SÉCURITÉ SFML 3 : Le constructeur par défaut de sf::Sound n'existe plus, lien direct requis
{
    // Chargement de la texture graphique de la tourelle
    if (m_texture.loadFromFile(texturePath)) {
        m_sprite.setTexture(m_texture, true); // Recalcule l'enveloppe rectangulaire en SFML 3
        auto textureSize = m_texture.getSize();
        m_sprite.setTextureRect(sf::IntRect({0, 0}, {static_cast<int>(textureSize.x), static_cast<int>(textureSize.y)}));
        
        // Configuration du point pivot d'ancrage (Origine centrée)
        auto bounds = m_sprite.getLocalBounds();
        m_sprite.setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        m_sprite.setScale({0.08f, 0.08f}); // Redimensionnement pour correspondre à l'échelle de la map
        m_sprite.setPosition(position);
    } else {
        std::cerr << "[ERREUR VISUEL] Impossible de charger l'asset de la tour : " << texturePath << "\n";
    }

    // Sélection du fichier audio dynamique selon la nature élémentaire de la tour
    std::string soundPath = "asset/sound/tir_defaut.wav"; 
    if (typeTour == "Feu") soundPath = "asset/sound/tir_feu.wav";
    else if (typeTour == "Glace") soundPath = "asset/sound/tir_glace.wav";

    // Chargement du buffer et configuration de l'effet sonore
    if (m_soundBuffer.loadFromFile(soundPath)) {
        m_sound.setBuffer(m_soundBuffer);
        m_sound.setVolume(50.f); // Réduction du volume à 50% pour préserver les oreilles
    } else {
        std::cerr << "[ERREUR AUDIO] Échec du chargement de l'effet sonore : " << soundPath << "\n";
    }
}

void TourView::jouerSonTir() {
    m_sound.play(); // Lecture asynchrone dans un thread matériel séparé (natif SFML)
}

void TourView::draw(sf::RenderWindow &window) const {
    window.draw(m_sprite); // Affichage sur la fenêtre de rendu passée par référence
}
