#include "Bullet.h"
#include <iostream>
#include <math.h>
#include <SFML/Graphics.hpp>

Bullet::Bullet(float x, float y, float angle, sf::Texture& texture)
{
    sprite.setTexture(texture);
    sprite.setOrigin(texture.getSize().x / 2.f, texture.getSize().y / 2.f);
    sprite.setScale(0.3f, 0.3f);
    sprite.setPosition(x, y);
    sprite.setRotation(angle + 90.f);

    float radians = angle * (3.14159f / 180.f);
    velocity = sf::Vector2f(std::cos(radians), std::sin(radians)) * speed;

    lifetime = 3.0f;
}

sf::Sprite& Bullet::getSprite(){
    return sprite;
}

void Bullet::calculateLifetime(float deltaTime){
    lifetime -= deltaTime;
    if (lifetime <= 0){
        kill();
    }
}

void Bullet::kill(){
    alive = false;
    std::cout << "A bullet has been deleted" << std::endl;
}

bool Bullet::isAlive() const {
    return alive;
}

void Bullet::update(float deltaTime)
{
    sprite.move(velocity * deltaTime);
    calculateLifetime(deltaTime);
}

void Bullet::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}