#ifndef MENU_H
#define MENU_H
#include <SFML/Graphics.hpp>

class Menu
{
	public:
		Menu(){
			menuBox.setSize(sf::Vector2f(300, 200));
		}
		~Menu();
		void addOption(std::string option){
			sf::Font
			sf::Text text(option);
			menuOptions.push_back(text);
		}

	private:
		std::vector<sf::Text> menuOptions;
		sf::RectangleShape menuBox;
};

#endif // MENU_H
