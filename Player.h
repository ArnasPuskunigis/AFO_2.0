// Player.h
#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include <memory>
#include "Enemy.h"
#include "Bullet.h"
#include "EventSystem.h"

class Player {
public:
    Player(sf::Texture& texture, EventSystem& events);
    void handleInput(float deltaTime);
    void update(float deltaTime, std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Bullet>& enemyBullets);
    void draw(sf::RenderWindow& window);
    void checkForEnemyCollisions(std::vector<std::unique_ptr<Enemy>>& enemies, std::vector<Bullet>& enemyBullets);
    void shootBullet();
    void takeDamage();
    void receiveBullets();
    void receiveHealth();
    void addScore(int amount);
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
    int health = 200;
    int maxAmmo = 20;
    int ammo = 20;
    sf::Sprite sprite;
    float speed = 150.0f;
};