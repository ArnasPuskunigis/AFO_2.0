#pragma once

#include <SFML/Graphics.hpp>
#include "Bullet.h"

class Enemy {
public:
    Enemy(float x, float y, sf::Texture& texture);
    void update(float deltaTime, std::vector<Bullet>& bullets);
    void draw(sf::RenderWindow& window);
    void checkForBulletCollisions(std::vector<Bullet>& bullets);

private:
    sf::Sprite sprite;
};