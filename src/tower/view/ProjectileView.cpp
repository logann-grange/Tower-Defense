#include "ProjectileView.hpp"

ProjectileView::ProjectileView(const Projectile& logique)
    : m_sprite(ResourceManager::getInstance().getTexture(
          choisirTexture(logique.getType(), logique.getNiveau())))
{
    float echelle = choisirEchelle(logique.getType(), logique.getNiveau());

    auto size = m_sprite.getTexture().getSize();
    m_sprite.setOrigin({static_cast<float>(size.x) / 2.f,
                        static_cast<float>(size.y) / 2.f});
    m_sprite.setScale({echelle, echelle});
    m_sprite.setPosition(logique.getPosition());
}

std::string ProjectileView::choisirTexture(const std::string& type, int niveau) {
    if (type == "Feu") {
        if (niveau == 2) return "assets/Projectile/fire/projectile_feu_4.png";
        if (niveau >= 3) return "assets/Projectile/fire/projectile_feu_6.png";
        return "assets/Projectile/fire/projectile_feu_1.png";
    }
    if (type == "Glace") {
        if (niveau == 2) return "assets/Projectile/ice/projectile_glace_2.png";
        if (niveau >= 3) return "assets/Projectile/ice/projectile_glace_3.png";
        return "assets/Projectile/ice/projectile_glace_1.png";
    }
    return "assets/Projectile/fire/projectile_feu_1.png"; // fallback
}

float ProjectileView::choisirEchelle(const std::string& type, int niveau) {
    if (type == "Feu") {
        if (niveau == 2) return 0.45f;
        if (niveau >= 3) return 0.30f;
        return 0.35f;
    }
    if (type == "Glace") {
        if (niveau == 2) return 0.18f;
        if (niveau >= 3) return 0.22f;
        return 0.25f;
    }
    return 0.35f;
}

void ProjectileView::update(const Projectile& logique) {
    m_sprite.setPosition(logique.getPosition());
    m_sprite.setRotation(sf::degrees(logique.getAngle()));
}

void ProjectileView::draw(sf::RenderWindow& window) const {
    window.draw(m_sprite);
}
