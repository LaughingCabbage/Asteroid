#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "game.h"
#include "Projectile.h"
#include <iostream>
#include <ctime>
#include "stdlib.h"
#include "Obstacle.h"
#include "Ship.h"
#include <string>
#include <sstream>
#include <cmath>


class game
{
	public:
		game(sf::RenderWindow* window){
			//gameWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "Asteroid");
			//gameWindow->setFramerateLimit(60);
			this->gameWindow = window;
			this->gameScore = 0;
		}
		~game(){}

		void setup(){
			gameState = play;
			if(!shipTextures.loadFromFile("planes.png"))
				std::cout << "planes.png failed to load.\n";
			ship.setTexture(shipTextures);
			ship.setTextureRect(sf::IntRect(161, 303, 29, 31));
			ship.setPosition(400.0, 500.0);
			ship.setOrigin((ship.getLocalBounds().width)/2, (ship.getLocalBounds()).height/2);
			projectile.setFillColor(sf::Color::White);
			if(!gameFont.loadFromFile("pixelmix.ttf"))
				std::cout << "pixelmix font failed to load.\n";
			score.setFont(gameFont);

            int livesStartX = 700;
			int livesStartY = 575;
			ship.setPosition(livesStartX, livesStartY);
			shipLives.push_back(ship);
			ship.setPosition(livesStartX + ship.getTextureRect().width, livesStartY);
			shipLives.push_back(ship);
			ship.setPosition(livesStartX + (2*ship.getTextureRect().width), livesStartY);
			shipLives.push_back(ship);
			for(std::size_t i = 0; i < 3; i++){
                shipLives.at(i).scale(.8,.8);
			}
			//set beginning position
			ship.setPosition(400.0, 500.0);

			score.setScale(0.5, 0.5);
			score.setString("0000000000"); //eight digit score
			score.setColor(sf::Color::White);

		}

		void gameLoop(){
			while (gameWindow->isOpen()){
				parseInput();
				if(gameState == play){
					updateProjectiles();
					generateObstacle();
					updateObstacles();
					checkCollision();
				} else if(gameState == endLevel){
					//displayResults();
				}
				renderFrame();
			}
		}

		void renderFrame(){
			//clear frame
			gameWindow->clear();
			//handle play state
				//draw projectiles
				if(!projectiles.empty()){
					for(std::size_t i = 0; i < projectiles.size(); i++){
						gameWindow->draw(*projectiles[i]);
					}
				}
				//draw obsticles
				if(!obstacles.empty()){
					for(std::size_t i = 0; i < obstacles.size(); i++){
						gameWindow->draw(*obstacles[i]);
					}
				}

				for(std::size_t i = 0; i < ship.getLives(); i++){
                    gameWindow->draw(shipLives.at(i));
				}

				gameWindow->draw(score);
				gameWindow->draw(ship);
			//display changes.
			gameWindow->display();

		}
		void parseInput(){
			sf::Event evt;
			//Loop through all window events
			while(gameWindow->pollEvent(evt)){
				if(evt.type == sf::Event::Closed)
					gameWindow->close();
			}
			//check for movement every 200 ms. Limits Ship speed.
			if(shipMovementClock.getElapsedTime().asMilliseconds() >= 1 ){
				shipMovementClock.restart();
				getMovement();
			}
			//fire on space.
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Space)){
				fireProjectile();
			}
		}

		void getMovement(){
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
				if(ship.getPosition().x > 20)
					ship.move(-1.0, 0.0);
			}
			if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
				if(ship.getPosition().x < 780)
					ship.move(1.0, 0.0);
			}

		}

		void fireProjectile(){
			//fire rate at 5 shots per second
			if(fireRateClock.getElapsedTime().asMilliseconds() > 200){
				fireRateClock.restart();
				Projectile *proj = new Projectile(ship.getPosition());
				//std::cout << "Projectile Fired.\n";
				projectiles.push_back(proj);
			}
		}

		void generateObstacle(){
			//seed random number generator
			srand(time(NULL));
			//generate obstacle at 25% chance per second
			int generationRate = rand() % 100 + 1;
			if(obstacleTimer.getElapsedTime().asSeconds() > 1 && generationRate ){
				//std::cout<< "Generating Obstacle\n";
				obstacleTimer.restart();
				Obstacle *obstacle;
				//Determine type of obstacle at random.
				int Px = rand() % 100 + 1;
				if(Px > 0 ){
					obstacle = new Obstacle;
					int location = rand() % 600 + 100;
					obstacle->setPosition(location, -10);
					obstacles.push_back(obstacle);
					obstacle = NULL;
				}
			}
		}

		void updateObstacles(){
			if(!obstacles.empty() && obstacleTimer.getElapsedTime().asSeconds() > 0.015){
				obstacleTimer.restart();
				for(std::vector<Obstacle*>::iterator i = obstacles.begin(); i != obstacles.end(); ++i){
					//delete obsticles out of bounds
					(*i)->move(0, 5.0);
					if((*i)->getPosition().y > 600){
						Obstacle *obj = (*i);
						if(obj != NULL)
							delete obj;
						obstacles.erase(i);
						//std::cout << "Obstacle Erased.\n";
						if(i == obstacles.end())
							--i;
					}
				}
			}
		}

		void checkCollision(){
			//only check when things exist.
			std::vector<Projectile*>::iterator projIt;
			std::vector<Obstacle*>::iterator obstIt;
			if(!projectiles.empty() && !obstacles.empty()){
				for(projIt = projectiles.begin(); projIt != projectiles.end(); ++projIt){
					for(obstIt = obstacles.begin(); obstIt != obstacles.end(); ++obstIt){
						if((*projIt)->getGlobalBounds().intersects((*obstIt)->getGlobalBounds())){
							//remove projectile and obstacle upon collision
							Projectile *proj = (*projIt);
							delete proj;
							projectiles.erase(projIt);

							//only remove obstacles when hit count has reached zero
							if((*obstIt)->updateHitCount()){
								Obstacle *obst = (*obstIt);
								updateScore(obst->getPoints());
								if(obst != NULL)
									delete obst;
								obstacles.erase(obstIt);
							}
							//only check for a single collision on any given loop.
							return;
						}
					}
				}
			}
			//check all obstacles for a collision with the ship
			for(obstIt = obstacles.begin(); obstIt != obstacles.end(); ++obstIt){
				if((*obstIt)->getGlobalBounds().intersects(ship.getGlobalBounds())){
					std::cout << "ship collision.\n";
					ship.updateLives(-1);
					if(ship.getLives() == 0)
						gameState = endLevel;
					if((*obstIt) != NULL){
						delete (*obstIt);
					}
					obstacles.erase(obstIt);
					break;
				}
			}
		}

		void updateScore(int ammount){
			std::string newScore;
			gameScore += ammount;
			std::stringstream out;
			out << gameScore;
			newScore = out.str();
			for(int i = 0; i <= 8; i++){
				if(gameScore <= pow(10, i)){
					newScore = "0" + newScore;
				}
			}
			score.setString(newScore);
		}

		void updateProjectiles(){
			if(!projectiles.empty()){
				obsticleTimer.restart();
				for(std::vector<Projectile*>::iterator i = projectiles.begin(); i != projectiles.end(); ++i){
						(*i)->updateProjectile();
					if((*i)->getPosition().y < 0.0){
						Projectile* proj = (*i);
						if (proj != NULL){
							delete proj;
						}
						projectiles.erase(i);
						//std::cout << "Projectile Erased.\n";
						if(i == projectiles.end()){
							break;
						}
					}
				}
			}
		}

		void displayResults(){
			std::cout<<"displaying results\n";
		}


	private:
		enum GameState{play, pause, endLevel, exit};
		GameState gameState;
		sf::RenderWindow *gameWindow;
		Ship ship;
		std::vector<Ship> shipLives;
		sf::Clock shipMovementClock;
		sf::Clock fireRateClock;
		sf::Clock obsticleTimer;
		sf::RectangleShape projectile;
		std::vector<Projectile*> projectiles;
		std::vector<Obstacle*> obstacles;

		sf::Clock obstacleTimer;
		sf::Text score;
		sf::Font gameFont;
		sf::Texture shipTextures;
		int gameScore;

};

#endif // GAME_H
