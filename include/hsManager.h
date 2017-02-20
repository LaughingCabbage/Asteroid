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

		void writeScore(int player_score, std::string player_name){
            std::ofstream ofs("hsList.dat", std::ios::app);
            if(ofs.is_open()){
                ofs << player_name << " " << player_score << "\n";
            }
            ofs.close();
		}


		void setWindow(sf::RenderWindow *rwindow){
            window = rwindow;
		}

		void loadScores(){
		    std::cout << "loading scores\n";
			std::ifstream ifs;
			ifs.open("hsList.dat");
			if(ifs.is_open()){
				std::string buffer = "";
				while(!ifs.eof()){
                    ifs >> buffer;
                    names.push_back(sf::Text(buffer, m_font));
                    ifs >> buffer;
                    scores.push_back(sf::Text(buffer, m_font));
				}
				ifs.close();
			}
		}

		void display(){
		    std:: cout << "displaying\n";

            int top = 0;
            float upperLeftX = 150;
            float upperLeftY = 100;
            for(int i = 0; i < scores.size(); i++){
                std::cout << "setting score " << i+1 << std::endl;
                names.at(i).setPosition(sf::Vector2f(upperLeftX, upperLeftY));
                scores.at(i).setPosition(sf::Vector2f(upperLeftX+100, upperLeftY));
                upperLeftY += 50;
                if(upperLeftY > 600) break;
                if(upperLeftX > 600) break;
                top++;
            }
            std::cout << "TOP = " << top << std::endl;


            int flag = 0;
            while(!flag){
                window->clear();

                ///draw scores
                if(!scores.empty()){
                    for(size_t i = 0; i < top; ++i){
                        window->draw(scores[i]);
                        window->draw(names[i]);
                    }
                }
                window->display();

                sf::Event event;
                while(window->pollEvent(event)){
                    if((event.type == sf::Event::KeyPressed) && (event.key.code == sf::Keyboard::Escape)){
                        flag = 1;
                    }

                }
			}
		}

	private:
		sf::RenderWindow *window;
		sf::Font m_font;
		std::vector<sf::Text> scores;
		std::vector<sf::Text> names;
};



#endif // HSMANAGER_H
