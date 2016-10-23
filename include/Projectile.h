#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <SFML/Graphics.hpp>
#include <iostream>

class Projectile : public sf::CircleShape
{
	public:
		Projectile(const sf::Vector2f& shipLocation);
		~Projectile();
		void updateProjectile();

	private:
		sf::Clock projectileClock;
};

#endif // PROJECTILE_H
