#include "ShootingEnemy.h"
#include <cmath>

ShootingEnemy::ShootingEnemy(float x, float y, sf::Texture& texture, sf::Texture& bulletTexture,
    AudioManager& audioManager, EventSystem& events, std::vector<Bullet>& enemyBullets)
    : Enemy(x, y, texture, audioManager, events), bulletTexture(bulletTexture), enemyBullets(enemyBullets)
{
    health = 10;
    speed = 150.f;
}

void ShootingEnemy::update(float deltaTime, std::vector<Bullet>& playerBullets, sf::Vector2f playerPos)
{
    if (!alive) return;

    Enemy::update(deltaTime, playerBullets, playerPos);

    shootTimer += deltaTime;
    if (shootTimer >= shootInterval)
    {
        shootTimer = 0.f;
        shoot(playerPos);
    }
}

void ShootingEnemy::shoot(sf::Vector2f playerPos)
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    sf::Vector2f spawnPos(
        bounds.left + bounds.width / 2.f,
        bounds.top + bounds.height
    );
    sf::Vector2f direction = playerPos - spawnPos;
    float angle = std::atan2(direction.y, direction.x) * (180.f / 3.14159f);
    enemyBullets.push_back(Bullet(spawnPos.x, spawnPos.y, angle, bulletTexture));
}