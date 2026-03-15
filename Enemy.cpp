#include "Enemy.h"
#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Enemy::Enemy(float x, float y, sf::Texture &texture)
{
    sprite.setTexture(texture);
    sprite.setScale(0.2f, 0.2f);
    sprite.setPosition(x, y);
}

void Enemy::checkForBulletCollisions(std::vector<Bullet>& bullets)
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    for (Bullet &Bullet : bullets)
    {
        if (bounds.intersects(Bullet.getSprite().getGlobalBounds()) && Bullet.getCanCollide() == true)
        {
            Bullet.disableCollision();
            std::cout << "A bullet has hit this enemy!" << std::endl;
        }
    }
}

void Enemy::update(float deltaTime, std::vector<Bullet>& bullets)
{
    checkForBulletCollisions(bullets);
}

void Enemy::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}