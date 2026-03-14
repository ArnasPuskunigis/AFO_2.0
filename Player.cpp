#include "Player.h"

Player::Player(sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.5f, 0.5f);
}

void Player::handleInput(float deltaTime) {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            sprite.move(sf::Vector2f(150, 0) * deltaTime);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            sprite.move(sf::Vector2f(-150, 0) * deltaTime);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            sprite.move(sf::Vector2f(0, -150) * deltaTime);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            sprite.move(sf::Vector2f(0, 150) * deltaTime);
        }
}

void Player::update(float deltaTime) {
    handleInput(deltaTime);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}