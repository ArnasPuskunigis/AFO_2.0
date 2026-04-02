#include "Pickup.h"
#include <iostream>

Pickup::Pickup(float x, float y, sf::Texture &texture, PickupType type)
{
    sprite.setTexture(texture);
    sprite.setScale(0.03f, 0.03f);
    sprite.setPosition(x, y);
    pickupType = type;
}

void Pickup::checkForPlayerCollision(Player &player)
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    if (bounds.intersects(player.getSprite().getGlobalBounds()))
    {
        if (pickupType == PickupType::Ammo) {
            player.receiveBullets();
            std::cout << "The player got ammo!" << std::endl;
        }
        else {
            player.receiveHealth();
            std::cout << "The player got health!" << std::endl;
        }
        kill();
    }
}

bool Pickup::isAlive() const {
    return alive;
}

void Pickup::kill(){
    alive = false;
    std::cout << "Pickup destroyed!" << std::endl;
}

void Pickup::update(float deltaTime, Player &player)
{
    checkForPlayerCollision(player);
}

void Pickup::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}