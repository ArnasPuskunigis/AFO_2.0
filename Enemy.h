#pragma once

#include <SFML/Graphics.hpp>
#include "Bullet.h"

class Enemy {
public:
    Enemy(float x, float y, sf::Texture& texture);
    void update(float deltaTime, std::vector<Bullet>& bullets);
    void draw(sf::RenderWindow& window);
    void checkForBulletCollisions(std::vector<Bullet>& bullets);
    void takeDamage();
    void kill();

    bool isAlive() const;
    sf::Sprite& getSprite();

private:
    int health = 20;
    sf::Sprite sprite;
    bool alive =  true;
};