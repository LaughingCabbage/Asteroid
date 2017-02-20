#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>
#include <iostream>
#include "game.h"
#include "hsManager.h"


enum menuOption{play, score, options};

class MenuButton{
	public:
		//constructor
		MenuButton(sf::Text& text, sf::RectangleShape& box);
		void setPosition(sf::Vector2f position);
		void drawButton(sf::RenderWindow* window);
		void select();
		void clearSelect();

	private:
		sf::Text buttonText;
		sf::RectangleShape buttonBox;
};

class Menu
{
	public:
		Menu(sf::RenderWindow *window);
		~Menu();
		void setup();
		void addOption(std::string option, sf::Vector2f position);
		void renderMenu();
		void selectUp();
		void selectDown();
		void playGame();
		void showHighScore();
		void selectOption();
		void parseMenuInput();
		void setWindow(sf::RenderWindow* window);

	private:
		sf::RenderWindow* menuWindow;
		std::vector<MenuButton> menuButtons;
		sf::Font menuFont;
		menuOption menuOptions;
		sf::Clock inputClock;
		float inputRate;
		hsManager HsManager;


};

#endif // MENU_H
