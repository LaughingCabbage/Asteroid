#ifndef SHIP_H
#define SHIP_H
#include <SFML/Graphics.hpp>
#include <iostream>

class Ship : public sf::Sprite
{
	public:
		Ship();
		~Ship();
		int getLives();
		void updateLives(int delta);

	private:
		int lives;

};

#endif // SHIP_H
