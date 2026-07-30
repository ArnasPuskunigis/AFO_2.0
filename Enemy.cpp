#include "Enemy.h"
#include "Bullet.h"
#include <SFML/Graphics.hpp>
#include <iostream>

Enemy::Enemy(float x, float y, sf::Texture& texture, AudioManager& audioManager, EventSystem& events) 
    : audio(audioManager), events(events)
{
    sprite.setTexture(texture);
    sprite.setScale(0.2f, 0.2f);
    sprite.setPosition(x, y);
}

void Enemy::checkForBulletCollisions(std::vector<Bullet>& bullets)
{
    sf::FloatRect bounds = sprite.getGlobalBounds();

    for (Bullet &Bullet : bullets)
    {
        if (bounds.intersects(Bullet.getSprite().getGlobalBounds()) && Bullet.isAlive() == true)
        {
            Bullet.kill();
            takeDamage();
            std::cout << "A bullet has hit this enemy!" << std::endl;
        }
    }
}

sf::Sprite& Enemy::getSprite(){
    return sprite;
}

void Enemy::takeDamage(){
    health -= 10;
    if (health <= 0){
		audio.play("enemyExplosion", 20);
        kill();
    }
}

bool Enemy::isAlive() const {
    return alive;
}

void Enemy::kill(){
    alive = false;
    events.emit(GameEvent::EnemyKilled);
    std::cout << "This enemy ship has been destroyed" << std::endl;
}

void Enemy::update(float deltaTime, std::vector<Bullet>& bullets, sf::Vector2f playerPos)
{
    if (!alive) return;

    // rotate to face player
    sf::Vector2f direction = playerPos - sprite.getPosition();
    float angle = std::atan2(direction.y, direction.x) * (180.f / 3.14159f);
    sprite.setRotation(angle - 90.f);

    // move towards player
    float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
    if (length > 0)
    {
        sf::Vector2f normalized = direction / length;
        sprite.move(normalized * speed * deltaTime);
    }

    checkForBulletCollisions(bullets);
}

void Enemy::draw(sf::RenderWindow &window)
{
    window.draw(sprite);
}