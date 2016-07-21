#ifndef SHIP_H
#define SHIP_H
#include <SFML/Graphics.hpp>
#include <iostream>

class Ship : public sf::Sprite
{
	public:
		Ship(){
			//start with 3 lives.
			this->lives = 3;
		}
		~Ship(){}
		int getLives(){
			return lives;
		}
		void updateLives(int delta){
			lives += delta;
			std::cout << "Lives: " << lives << std::endl;
			if(lives == 0)
				std::cout << "GAME OVER\n";
		}

	private:
		int lives;

};

#endif // SHIP_H
