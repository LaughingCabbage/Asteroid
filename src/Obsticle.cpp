#include "Obsticle.h"

Obstacle::Obstacle(){
    srand(time(NULL));
    int width = rand() % 100 + 20;
    int height = rand() % 40 + 15;
    this->setSize(sf::Vector2f(width,height));
    this->hitCount = 3;
    this->setFillColor(sf::Color::Green);
}

Obstacle::~Obstacle(){}

bool Obstacle::updateHitCount(){
    //updates hit count and return true if object is to be removed.
    hitCount--;
    switch (hitCount){
        case 3:
            this->setFillColor(sf::Color::Green);
            break;
        case 2:
            this->setFillColor(sf::Color::Yellow);
            break;
        case 1:
            this->setFillColor(sf::Color::Red);
            break;
        case 0:
            return true;
        default:
            break;
    }
    return false;
}
//returns number of points for a given object according to area
int Obstacle::getPoints(){
    int area = this->getSize().x * this->getSize().y;
    if(area <= 100){
        return 5000;
    }else if(area <= 200){
        return 4000;
    }else if(area <= 300){
        return 3000;
    }else if(area <= 400){
        return 2000;
    }else{
        return 1000;
    }
    //return -1 if issues
    return -1;
}
