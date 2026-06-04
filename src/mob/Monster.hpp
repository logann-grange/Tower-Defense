#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

class Monster
{
public:
    Monster() : health(0), maxHealth(0), speed(0.0f), damage(0), reward(0), x(0), y(0) {}
    virtual ~Monster() = default;
    int getHealth() const
    {
        return health;
    }

    float getSpeed() const
    {
        return speed;
    }

    int getDamage() const
    {
        return damage;
    }

    int getReward() const
    {
        return reward;
    }

    int getX() const
    {
        return x;
    }
    int getY() const
    {
        return y;
    }

    void takeDamage(int amount);

    bool isDead() const;

    void setPosition(int nx, int ny);

    void heal(int amount);

    virtual void move(float deltaTime) = 0;

    virtual void mourir() = 0;

    virtual void attaquer() = 0;

    void spawn(const std::vector<sf::Vector2i> &cheminPoints);
    // 💡 AJOUT : La fonction de base qui permet à la Tour de demander la position
    virtual sf::Vector2f getPosition() const
    {
        return sf::Vector2f(x * 16.f, y * 16.f);
    }

protected:
    int health;
    int maxHealth;
    float speed;
    int damage;
    int reward;
    int x;
    int y;
    int dmg;
    std::string faiblesse;
    std::string lienSprit;
};

#endif // MONSTER_HPP