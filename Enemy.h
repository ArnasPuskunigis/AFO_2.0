#pragma once
#include <SFML/Graphics.hpp>
#include "AudioManager.h"
#include "EventSystem.h"
#include <vector>

class Bullet;

class Enemy {
public:
    Enemy(float x, float y, sf::Texture& texture, AudioManager& audioManager, EventSystem& events);
    virtual void update(float deltaTime, std::vector<Bullet>& bullets, sf::Vector2f playerPos);
    void draw(sf::RenderWindow& window);
    void takeDamage();
    bool isAlive() const;
    sf::Sprite& getSprite();

private:
    void checkForBulletCollisions(std::vector<Bullet>& bullets);
    void kill();

    AudioManager& audio;
    EventSystem& events;
    
protected:
    bool alive = true;
    sf::Sprite sprite;
    int health = 20;
    float speed = 300;
};