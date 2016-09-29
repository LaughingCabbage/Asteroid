#include <SFML/Graphics.hpp>
#include "game.h"
#include "Menu.h"

int main(){
	//create window for display
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode(800, 600), "Asteroid");
	//window->setFramerateLimit(60);
	//create menu state for display
    Menu gameMenu(window);
    while(window->isOpen()){
		gameMenu.renderMenu();
    }

    delete window;
	//Asteroid.setup();
	//Asteroid.gameLoop();
	return 0;
}
