#pragma once
#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "EventSystem.h"

class Player {
public:
    Player(sf::Texture& texture, EventSystem& events);
    void handleInput(float deltaTime);
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies);
    void draw(sf::RenderWindow& window);
    void checkForEnemyCollisions(std::vector<std::unique_ptr<Enemy>>& enemies);
    void shootBullet();
    void takeDamage();
    void receiveBullets();
    void receiveHealth();
    void addScore(int amount);
    void incrementKills();
    int getScore();
    int getHealth() { return health; }
    int getAmmo() { return ammo; }
    sf::Vector2f getWeaponPosition();
    sf::Sprite& getSprite();

private:
    void setupEventListeners();

    EventSystem& events;
    int score = 0;
    int maxHealth = 200;
    int health;
    int maxAmmo = 20;
    int ammo;
    sf::Sprite sprite;
    float speed = 150.0f;
};