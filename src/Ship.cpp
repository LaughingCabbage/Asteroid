#include "Ship.h"

Ship::Ship(){
    //start with 3 lives.
    this->lives = 3;
}
Ship::~Ship(){}

int Ship::getLives(){
    return lives;
}

void Ship::updateLives(int delta){
    lives += delta;
    std::cout << "Lives: " << lives << std::endl;
    if(lives == 0)
        std::cout << "GAME OVER\n";
}
