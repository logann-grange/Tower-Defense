#include "Castle.hpp"

Castle::Castle() : m_health(20), m_maxHealth(20) {}

void Castle::initialiser(sf::Vector2i position) {
    m_position = position;
}

void Castle::takeDamage(int damage) {
    m_health -= damage;
    if (m_health < 0) m_health = 0;
}

bool Castle::isDestroyed() const {
    return m_health <= 0;
}

int Castle::getHealth() const {
    return m_health;
}