#include "Enemy.h"

Enemy::Enemy(float x, float y, sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.2f, 0.2f);
    sprite.setPosition(x, y);
}

void Enemy::update(float deltaTime) {

}

void Enemy::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}