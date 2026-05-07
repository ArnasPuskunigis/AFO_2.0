#pragma once

#include <SFML/Graphics.hpp>
#include "Bullet.h"
#include "AudioManager.h"

class Enemy {
public:
    Enemy(float x, float y, sf::Texture& texture, AudioManager& audio);
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
    AudioManager& audio;
};