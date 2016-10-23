#include "Projectile.h"

Projectile::Projectile(const sf::Vector2f& shipLocation){
    this->setRadius(1.0);
    this->setFillColor(sf::Color::White);
    this->setPosition(shipLocation);
    this->setOrigin(this->getLocalBounds().width/2, this->getLocalBounds().height/2);
}

Projectile::~Projectile(){}

void Projectile::updateProjectile(){
    this->move(0.0, -1.0);
}
