#include "Pickup.h"
#include <iostream>

Pickup::Pickup(float x, float y, sf::Texture &texture, PickupType type)
{
    sprite.setTexture(texture);
    sprite.setScale(0.03f, 0.03f);
    sprite.setPosition(x, y);
}

bool Pickup::isAlive() const {
    return alive;
}

void Pickup::kill(){
    alive = false;
    std::cout << "Pickup collected!" << std::endl;
}

void Pickup::update(float deltaTime)
{

}

void Pickup::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}