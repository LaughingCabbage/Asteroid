#ifndef PAUSEMENU_H
#define PAUSEMENU_H

#include "Menu.h"

/*
class pauseMenu
{
    public:
        pauseMenu(){
            if(!menuFont.loadFromFile("pixelmix.ttf")){
                std::cout << "font load error\n";
            }
            MenuButton button(sf::Text("Play",menuFont),
                               sf::RectangleShape(sf::Vector2f(200,300)));
            pauseButtons.push_back(button);
            MenuButton button2(sf::Text("Quit",menuFont),
                               sf::RectangleShape(sf::Vector2f(200,300)));
        }
        ~pauseMenu(){}

        void addOption(std::string option, sf::Vector2f position){
            sf::RectangleShape box(sf::Vector2f(200, 100));
            box.setFillColor(sf::Color::Yellow);
            sf::Text text(option, menuFont);
            //center text
            text.setOrigin(text.getLocalBounds().width/2, text.getLocalBounds().height/2);
            text.setColor(sf::Color::Black);
            MenuButton button(text, box);
            button.setPosition(position);
            menuButtons.push_back(button);

        }

    private:
        sf::FloatRect pauseBox;
        std::vector<MenuButton> pauseButtons;
        sf::Font menuFont;
};

*/
#endif // PAUSEMENU_H
