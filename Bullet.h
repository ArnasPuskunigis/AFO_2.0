#pragma once

#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(float x, float y, float angle, sf::Texture& texture);
    void flyForwards(float deltaTime);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void disableCollision();
    bool getCanCollide();
    sf::Sprite& getSprite();

private:
    bool canCollide = true;
    sf::Sprite sprite;
    float speed = 500.0f;
    sf::Vector2f velocity;
};