#ifndef HSMANAGER_H
#define HSMANAGER_H


#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>

class hsManager
{
	public:
		hsManager(){
		//ctor
		}

		~hsManager(){
			//dtor
		}

		void setup(){
			m_font.loadFromFile("pixelmix.tff");
		}

		void setWindow(sf::RenderWindow *rwindow){
            window = rwindow;
		}

		void loadScores(){
			std::ifstream ifs("hsList.dat");
			if(ifs.is_open()){
				std::string buffer;
				while(std::getline(ifs, buffer)){
					sf::Text(buffer);
					scores.push_back(buffer);
				}
			}
		}
		void display(){
			window->clear();

			///draw scores
			if(!scores.empty()){
				for(size_t i = 0; i < scores.size(); ++i){
					window->draw(scores[i]);
				}
			}
			window->display();
		}

	private:
		sf::RenderWindow *window;
		sf::Font m_font;
		std::vector<sf::Text> scores;
};

#endif // HSMANAGER_H
