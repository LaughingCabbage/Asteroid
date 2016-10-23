#ifndef OBSTICLE_H
#define OBSTICLE_H
#include <SFML/Graphics.hpp>
#include "stdlib.h"
#include "time.h"
#include <iostream>

class Obstacle : public sf::RectangleShape
{
	public:
		Obstacle();
		~Obstacle();
		bool updateHitCount();
		//returns number of points for a given object according to area
		int getPoints();

	private:
		int hitCount;

};


#endif // OBSTICLE_H
