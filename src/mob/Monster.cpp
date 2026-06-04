#include "Monster.hpp"

// 💡 SOLUTION SFML 3 : Une texture statique vide partagée pour initialiser le sprite obligatoirement
static sf::Texture textureMonstreDefaut;

// ✅ Implémentation du constructeur avec la liste d'initialisation obligatoire pour m_sprite
Monster::Monster()
	: health(0), maxHealth(0), speed(0.0f), damage(0), reward(0),
	  x(0), y(0), dmg(0), m_sprite(textureMonstreDefaut)
{
}

void Monster::takeDamage(int amount)
{
	if (m_isDying)
		return;

	health -= amount;
	if (health <= 0)
	{
		health = 0;
		m_isDying = true;
	}
}

bool Monster::isDead() const
{
	return (health <= 0 && !m_isDying) || (m_alpha <= 0);
}

void Monster::updateDeathAnimation(float deltaTime)
{
	if (!m_isDying)
		return;

	m_deathTimer += deltaTime;

	float progression = m_deathTimer / m_maxDeathTime;
	m_alpha = static_cast<int>(255.f * (1.0f - progression));
	if (m_alpha < 0)
		m_alpha = 0;

	sf::Color color;
	if (static_cast<int>(m_deathTimer / 0.05f) % 2 == 0)
	{
		color = sf::Color(255, 80, 80, m_alpha);
	}
	else
	{
		color = sf::Color(255, 255, 255, m_alpha);
	}

	m_sprite.setColor(color);
}

void Monster::setPosition(int nx, int ny)
{
	x = nx;
	y = ny;
}

void Monster::heal(int amount)
{
	if (m_isDying)
		return;
	health += amount;
	if (health > maxHealth)
		health = maxHealth;
}