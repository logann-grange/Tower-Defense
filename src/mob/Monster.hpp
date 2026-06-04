#ifndef MONSTER_HPP
#define MONSTER_HPP

#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

class Monster
{
public:
    // ✅ CORRECTION SFML 3 : Le constructeur est maintenant déclaré ici et implémenté dans le .cpp
    Monster();
    virtual ~Monster() = default;

    int getHealth() const { return health; }
    float getSpeed() const { return speed; }
    int getDamage() const { return damage; }
    int getReward() const { return reward; }
    int getX() const { return x; }
    int getY() const { return y; }

    void takeDamage(int amount);
    bool isDead() const;
    bool isDying() const { return m_isDying; }

    void setPosition(int nx, int ny);
    void heal(int amount);

    virtual void move(float deltaTime) = 0;
    virtual void mourir() = 0;
    virtual void attaquer() = 0;

    void spawn(const std::vector<sf::Vector2i> &cheminPoints);
    
    virtual sf::Vector2f getPosition() const
    {
        return sf::Vector2f(x * 16.f, y * 16.f);
    }

    void updateDeathAnimation(float deltaTime);

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

    sf::Sprite m_sprite;         
    bool m_isDying{false};       
    float m_deathTimer{0.0f};    
    float m_maxDeathTime{0.6f};  
    int m_alpha{255};            
};

#endif // MONSTER_HPP