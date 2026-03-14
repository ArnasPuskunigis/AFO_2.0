#include "Bullet.h"
#include <iostream>

Bullet::Bullet(float x, float y, sf::Texture& texture)
{
    sprite.setTexture(texture);
    sprite.setPosition(x, y);
    sprite.setScale(0.5f, 0.5f);
}

void Bullet::flyForwards(float deltaTime)
{
    sprite.move(sf::Vector2f(speed, speed) * deltaTime);
}

void Bullet::update(float deltaTime)
{
    flyForwards(deltaTime);
}

void Bullet::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}