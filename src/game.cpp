#include "game.h"
#include <SFML/System.hpp>

#define DEADTIME 1 //as seconds

Game::Game(sf::RenderWindow* window){
    //gameWindow = new sf::RenderWindow(sf::VideoMode(800, 600), "Asteroid");
    //gameWindow->setFramerateLimit(60);
    this->gameWindow = window;
    this->gameScore = 0;
}
Game::~Game(){}

void Game::setup(){
    gameState = play;
    if(!shipTextures.loadFromFile("planes.png"))
        std::cout << "planes.png failed to load.\n";
    ship.setTexture(shipTextures);
    ship.setTextureRect(sf::IntRect(161, 303, 29, 31));
    ship.setPosition(400.0, 500.0);
    ship.setOrigin((ship.getLocalBounds().width)/2, (ship.getLocalBounds()).height/2);

    //ship.loadShip();

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

    //highScores.setWindow(gameWindow);

}

void Game::gameLoop(int &player_score, std::string &player_name){
    while (gameWindow->isOpen()){
        parseInput();
        if(gameState == play){
            //looks like a good place to implement some multithreading...
            updateProjectiles();
            generateObstacle();
            updateObstacles();
            checkCollision();
        } else if(gameState == exit){
            gameOver();
            player_score = gameScore;
            player_name = playerName;
            sf::Clock clk;
            while(clk.getElapsedTime().asSeconds() < 0.5); //1 second delay to avoid hitting play immediately after entering name

            return;
        }
        renderFrame();
    }
}

void Game::renderFrame(){
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
void Game::parseInput(){
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

void Game::getMovement(){
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left)){
        if(ship.getPosition().x > 20)
            ship.move(-1.0, 0.0);
    }
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)){
        if(ship.getPosition().x < 780)
            ship.move(1.0, 0.0);
    }

}

void Game::fireProjectile(){
    //fire rate at 5 shots per second
    if(fireRateClock.getElapsedTime().asMilliseconds() > 200){
        fireRateClock.restart();
        Projectile *proj = new Projectile(ship.getPosition());
        //std::cout << "Projectile Fired.\n";
        projectiles.push_back(proj);
    }
}

void Game::generateObstacle(){
    //seed random number generator
    srand(time(NULL));
    //generate obstacle at 25% chance per second

    if(generationRate.getElapsedTime().asSeconds() > 1 ){
        generationRate.restart();
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

void Game::updateObstacles(){
    if(!obstacles.empty() && obstacleTimer.getElapsedTime().asSeconds() > 0.015){
        obstacleTimer.restart();
        for(std::vector<Obstacle*>::iterator i = obstacles.begin(); i != obstacles.end(); ++i){
            //delete obstacles out of bounds
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

void Game::checkCollision(){
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
                gameState = exit;

            //cleanup so you don't get merked again
            if((*obstIt) != NULL){
                delete (*obstIt);
            }
            obstacles.erase(obstIt);

            //another one bites the dust.
            displayDeath();

            break;
        }
    }
}

void Game::updateScore(int ammount){
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

void Game::updateProjectiles(){
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

void Game::displayResults(){
    std::cout<<"displaying results\n";
}

void Game::displayDeath(){
    sf::Texture boom_texture;
    std::string file_name = "boom.png";
    sf::Sprite boom;
    if(!boom_texture.loadFromFile("boom.png")){
        std::cout << "failed to load boom\n";
    }else{
        boom.setTexture(boom_texture);
        boom.setPosition(ship.getPosition().x, ship.getPosition().y);
    }

    //delay between deaths
    sf::Clock deathClock;
    while(int(deathClock.getElapsedTime().asSeconds()) < DEADTIME){ //METAL
        gameWindow->clear();
        gameWindow->draw(boom);
        for(int i = 0; i < ship.getLives(); i++){
            gameWindow->draw(shipLives[i]);
        }
        gameWindow->display();
    }
}

void Game::gameOver(){

    sf::Sprite lose;
        std::string str = "GAME OVER";
        sf::Text txt(str, gameFont);
        txt.setPosition(gameWindow->getSize().y-gameWindow->getSize().y/2, gameWindow->getSize().x/2);

    sf::Clock game_over_clock;
    while(int(game_over_clock.getElapsedTime().asSeconds()) < DEADTIME*2){
        gameWindow->clear();
        gameWindow->draw(txt);
        gameWindow->display();

    }

    getName();

}

void Game::getName(){
    sf::RectangleShape box(sf::Vector2f(300, 100));
    box.setFillColor(sf::Color::Yellow);
    box.setOrigin(box.getSize().y/2, box.getSize().x/2);
    box.setPosition(gameWindow->getSize().y/2, gameWindow->getSize().x/2);
    sf::Text displayText("Enter your name: ", gameFont);
    displayText.setCharacterSize(14);
    displayText.setColor(sf::Color::Red);
    displayText.setPosition(box.getPosition().y - 140, box.getPosition().x - 40);


    sf::Text nameBuffer(playerName, gameFont);
    nameBuffer.setCharacterSize(16);
    nameBuffer.setPosition(box.getPosition().y - 130, box.getPosition().x);
    nameBuffer.setColor(sf::Color::Magenta);
    nameBuffer.setString(playerName);

    //poll user input of text
    bool flag = false;
    while(!flag){
        gameWindow->clear();
        gameWindow->draw(box);
        gameWindow->draw(displayText);
        gameWindow->draw(nameBuffer);
        gameWindow->display();

        sf::Event event;
        while(gameWindow->pollEvent(event)){
            switch(event.type){

                case sf::Event::TextEntered:
                    sf::String temp = nameBuffer.getString();
                    if(event.text.unicode == 0x08 && (temp.getSize() > 0 && temp.getSize() <= 12)){ //backspace removes a character
                        temp.erase(temp.getSize()-1, 1);
                        nameBuffer.setString(temp);
                        playerName = nameBuffer.getString();
                    }else if(event.text.unicode != 0x08){
                        sf::String character(static_cast<char>(event.text.unicode)); //letter adds a character
                        temp += character;
                        nameBuffer.setString(temp);
                        playerName = nameBuffer.getString();
                    }
            }
                //end Text Entered
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Return)){
            flag = true;
        }
    }
}
