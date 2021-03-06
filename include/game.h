#ifndef GAME_H
#define GAME_H
#include <SFML/Graphics.hpp>
#include <vector>
#include "Projectile.h"
#include <iostream>
#include "Menu.h"
#include <ctime>
#include "stdlib.h"
#include "Obstacle.h"
#include "Ship.h"
#include <string>
#include <sstream>
#include <cmath>
#include "hsManager.h"


class Game
{
	public:
		Game(sf::RenderWindow* window);
		~Game();
		void setup();
		void gameLoop(int &player_score, std::string &player_name);
		void renderFrame();
		void parseInput();
		void getMovement(); //from user
		void fireProjectile();
		void generateObstacle();
		void updateObstacles();
		void checkCollision();
		void updateScore(int ammount);
		void updateProjectiles();
		void displayResults();  //not doing anything atm
		void displayDeath();
		void gameOver();
        void getName();

	private:
		enum GameState{play, pause, endLevel, exit};
		GameState gameState;
		sf::RenderWindow *gameWindow;
		Ship ship;
		std::vector<Ship> shipLives;


		sf::Clock shipMovementClock;
		sf::Clock fireRateClock;
		sf::Clock obsticleTimer;
		sf::Clock generationRate;
		sf::RectangleShape projectile;
		std::vector<Projectile*> projectiles;
		std::vector<Obstacle*> obstacles;
		sf::Clock obstacleTimer;
		sf::Text score;
		sf::Font gameFont;
		sf::Texture shipTextures;
		int gameScore;
		std::string playerName;
		//hsManager highScores;


};

#endif // GAME_H
