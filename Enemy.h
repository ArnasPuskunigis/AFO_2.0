#pragma once
#include <SFML/Graphics.hpp>
#include "AudioManager.h"
#include "EventSystem.h"
#include <vector>

class Bullet;

class Enemy {
public:
    Enemy(float x, float y, sf::Texture& texture, AudioManager& audioManager, EventSystem& events);
    void update(float deltaTime, std::vector<Bullet>& bullets);
    void draw(sf::RenderWindow& window);
    void takeDamage();
    bool isAlive() const;
    sf::Sprite& getSprite();

private:
    void checkForBulletCollisions(std::vector<Bullet>& bullets);
    void kill();

    AudioManager& audio;
    EventSystem& events;
    sf::Sprite sprite;
    int health = 20;
    bool alive = true;
};