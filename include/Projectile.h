#ifndef PROJECTILE_H
#define PROJECTILE_H
#include <SFML/Graphics.hpp>
#include <iostream>

class Projectile : public sf::CircleShape
{
	public:
		Projectile(const sf::Vector2f& shipLocation){
			this->setRadius(1.0);
			this->setFillColor(sf::Color::White);
			this->setPosition(shipLocation);
			this->setOrigin(this->getLocalBounds().width/2, this->getLocalBounds().height/2);
		}
		~Projectile(){
		}

		void updateProjectile(){
			this->move(0.0, -1.0);
		}


	private:
		sf::Clock projectileClock;
};

#endif // PROJECTILE_H
