// Implementation file for Monster
// Monster is an abstract base class; its pure virtual methods
// are implemented by derived classes (e.g., Skeleton).

#include "Monster.hpp"

void Monster::takeDamage(int amount) {
	health -= amount;
	if (health < 0) health = 0;
}

bool Monster::isDead() const {
	return health <= 0;
}

void Monster::setPosition(int nx, int ny) {
	x = nx;
	y = ny;
}

void Monster::heal(int amount) {
	health += amount;
	if (health > maxHealth) health = maxHealth;
}

