#pragma once

#include <SFML/Graphics.hpp>
#include "Enemy.h"

class Player {
public:
    Player(sf::Texture& texture);
    void handleInput(float deltaTime);
    void update(float deltaTime, std::vector<Enemy>& enemies);
    void draw(sf::RenderWindow& window);
    void checkForEnemyCollisions(std::vector<Enemy>& enemies);
    void shootBullet();
    void takeDamage();
    void receiveBullets();
    void receiveHealth();

    int getHealth() { return health; }
    int getAmmo() { return ammo; }
    sf::Vector2f getWeaponPosition();

private:
    int maxHealth = 200;
    int health;
    int maxAmmo = 20;
    int ammo;
    sf::Sprite sprite;
    float speed = 150.0f;
};