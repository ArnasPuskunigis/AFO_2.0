#pragma once
#include "Enemy.h"
#include "Bullet.h"

class ShootingEnemy : public Enemy {
public:
    ShootingEnemy(float x, float y, sf::Texture& texture, sf::Texture& bulletTexture,
        AudioManager& audioManager, EventSystem& events,
        std::vector<Bullet>& enemyBullets);

    void update(float deltaTime, std::vector<Bullet>& playerBullets, sf::Vector2f playerPos) override;

private:
    void shoot(sf::Vector2f playerPos);

    sf::Texture& bulletTexture;
    std::vector<Bullet>& enemyBullets;
    float shootTimer = 0.f;
    float shootInterval = 2.f;
};