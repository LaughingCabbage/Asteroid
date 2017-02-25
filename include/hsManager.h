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

		}

		void setFont(sf::Font& gameFont){
            m_font = gameFont;
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
				std::string buffer2 = "";

				while(ifs >> buffer){
                    std::cout << "buffer: " << buffer << std::endl;
                    sf::String str(buffer);
                    names.push_back(sf::Text(str, m_font));
                    ifs >> buffer2;
                    std::cout << "buffer: " << buffer2 << std::endl;
                    sf::String str2(buffer2);
                    scores.push_back(sf::Text(str2, m_font));
				}
				std::cout << "done loading scores\n";
				ifs.close();
                std::cout << "size: " << scores.size() <<  std::endl;
                float upperLeftX = 150;
                float upperLeftY = 100;
                for(int i = 0; i < names.size(); i++){
                    std::cout << "setting score " << i+1 << std::endl;
                    names.at(i).setPosition(sf::Vector2f(upperLeftX, upperLeftY));
                    scores.at(i).setPosition(sf::Vector2f(upperLeftX+100, upperLeftY));
                    names[i].setCharacterSize(10);
                    scores[i].setCharacterSize(10);
                    upperLeftY += 50;
                    if(upperLeftY > 600) break;
                    if(upperLeftX > 600) break;
                }

                for(int i = 0; i < names.size(); ++i){
                    names[i].setColor(sf::Color::Yellow);
                    scores[i].setColor(sf::Color::Yellow);
                    int flag = 0;
                    if (names[i].getColor() == sf::Color::Yellow)
                        flag = 1;
                    std::cout << "color yellow : " << flag << std::endl;
				}



			}
		}

		void display(){
		    std:: cout << "displaying\n";

            int flag = 0;
            while(!flag){
                window->clear();

                ///draw scores
                if(!scores.empty()){
                    for(int i = 0; i < names.size(); ++i){
                       // std::cout << "score: " << scores[i].getString().toAnsiString() << std::endl;
                        window->draw(scores.at(i));
                        window->draw(names.at(i));
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
			std::cout << "end display\n";
		}

	private:
		sf::RenderWindow *window;
		sf::Font m_font;
		std::vector<sf::Text> scores;
		std::vector<sf::Text> names;
};



#endif // HSMANAGER_H
