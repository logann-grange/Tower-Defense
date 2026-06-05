#ifndef CASTLE_HPP
#define CASTLE_HPP

#include <SFML/System/Vector2.hpp>

class Castle {
public:
    Castle();
    void initialiser(const sf::Vector2i position);
    void takeDamage(int damage);
    bool isDestroyed() const;
    int getHealth() const;
    int getMaxHealth() const {return m_maxHealth;};
    sf::Vector2i getPosition() const { return m_position; }

private:
    int m_health;
    int m_maxHealth;
    sf::Vector2i m_position;
};

#endif // CASTLE_HPP