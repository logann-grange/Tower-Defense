#include "CastleView.hpp"

CastleView::CastleView()
    : m_sprite(ResourceManager::getInstance().getTexture("asset/Castle.png"))
{
    // Le sprite fait 1408x768 — on le redimensionne pour qu'il fasse
    // environ 96x96 pixels sur la carte (6 cases de 16px)
    sf::Vector2u texSize = ResourceManager::getInstance().getTexture("asset/Castle.png").getSize();
    float scaleX = 300.f / static_cast<float>(texSize.x);
    float scaleY = 230.f / static_cast<float>(texSize.y);
    m_sprite.setScale({scaleX, scaleY});

    // Origine au centre bas du sprite pour le poser sur la carte
    m_sprite.setOrigin({static_cast<float>(texSize.x) / 2.f, static_cast<float>(texSize.y)});

    // Barre de fond (rouge)
    m_barreFond.setSize({60.f, 6.f});
    m_barreFond.setFillColor(sf::Color::Red);
    m_barreFond.setOrigin({30.f, 0.f});

    // Barre de PV (verte)
    m_barrePV.setSize({60.f, 6.f});
    m_barrePV.setFillColor(sf::Color::Green);
    m_barrePV.setOrigin({30.f, 0.f});
}

void CastleView::update(const Castle& logique) {
    sf::Vector2f pos = {
        static_cast<float>(logique.getPosition().x)+70.f, // Décalage horizontal pour mieux centrer le sprite sur la tour
        static_cast<float>(logique.getPosition().y)+50.f  // Décalage vertical pour mieux aligner le sprite sur la tour
    };

    m_sprite.setPosition(pos);

    // Barre de vie au-dessus du château
    m_barreFond.setPosition({pos.x-115, pos.y - 150.f});
    m_barrePV.setPosition({pos.x-115, pos.y - 150.f});

    float ratio = static_cast<float>(logique.getHealth()) / static_cast<float>(logique.getMaxHealth());
    if (ratio < 0.f) ratio = 0.f;
    m_barrePV.setSize({60.f * ratio, 6.f});

    // La barre devient orange puis rouge selon les PV restants
    if (ratio > 0.5f) {
        m_barrePV.setFillColor(sf::Color::Green);
    } else if (ratio > 0.25f) {
        m_barrePV.setFillColor(sf::Color(255, 165, 0)); // orange
    } else {
        m_barrePV.setFillColor(sf::Color::Red);
    }
}

void CastleView::draw(sf::RenderWindow& window) {
    window.draw(m_sprite);
    window.draw(m_barreFond);
    window.draw(m_barrePV);
}