#include "TourView.hpp"

TourView::TourView(const Tour& logique, const std::string& texturePath)
    : m_sprite(m_texture), m_sound(m_soundBuffer)
{
    // Chargement texture
    if (m_texture.loadFromFile(texturePath)) {
        m_sprite.emplace(m_texture);
        auto size = m_texture.getSize();
        m_sprite->setTextureRect(sf::IntRect({0, 0},
            {static_cast<int>(size.x), static_cast<int>(size.y)}));
        auto bounds = m_sprite->getLocalBounds();
        m_sprite->setOrigin({bounds.size.x / 2.f, bounds.size.y / 2.f});
        m_sprite->setScale({0.08f, 0.08f});
        m_sprite->setPosition(logique.getPosition());
    } else {
        std::cerr << "[ERREUR] Texture tour introuvable : " << texturePath << "\n";
    }

    // Chargement son
    chargerSon(logique.getType());
}

void TourView::chargerSon(const std::string& type) {
    std::string soundPath = "assets/sounds/tir_defaut.wav";
    if (type == "Feu")   soundPath = "assets/sounds/tir_feu.wav";
    if (type == "Glace") soundPath = "assets/sounds/tir_glace.wav";

    if (m_soundBuffer.loadFromFile(soundPath)) {
        m_sound.setBuffer(m_soundBuffer);
        m_sound.setVolume(50.f);
    } else {
        std::cerr << "[WARNING] Son tour introuvable : " << soundPath << "\n";
    }
}

void TourView::update(const Tour& logique) {
    m_sprite->setPosition(logique.getPosition());
}

void TourView::draw(sf::RenderWindow& window) const {
    window.draw(*m_sprite);
}

TourView::TourView(const TourView& other)
   :  m_sprite(m_texture), m_sound(m_soundBuffer)

{
        m_sprite.emplace(m_texture);  // rebind sur la nouvelle texture
        // recopie toutes les transformations
        m_sprite->setTextureRect(other.m_sprite->getTextureRect());
        m_sprite->setOrigin(other.m_sprite->getOrigin());
        m_sprite->setScale(other.m_sprite->getScale());
        m_sprite->setPosition(other.m_sprite->getPosition());
}

TourView& TourView::operator=(const TourView& other) {
    if (this == &other) return *this;
    m_texture = other.m_texture;

    if (other.m_sprite.has_value()) {
        m_sprite.emplace(m_texture);
        m_sprite->setTextureRect(other.m_sprite->getTextureRect());
        m_sprite->setOrigin(other.m_sprite->getOrigin());
        m_sprite->setScale(other.m_sprite->getScale());
        m_sprite->setPosition(other.m_sprite->getPosition());
    }
    return *this;
}

sf::Sprite TourView::getSpriteCopy() const {
    return *m_sprite;
}
