#include "Player.h"

Player::Player(sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.2f, 0.2f);
}

sf::Vector2f Player::getWeaponPosition()
{
    sf::FloatRect bounds = sprite.getGlobalBounds();
    return sf::Vector2f(bounds.left + bounds.width / 2.f, bounds.top  + bounds.height);
}

void Player::handleInput(float deltaTime) {

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            sprite.move(sf::Vector2f(150, 0) * deltaTime);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            sprite.move(sf::Vector2f(-150, 0) * deltaTime);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W))
        {
            sprite.move(sf::Vector2f(0, -150) * deltaTime);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S))
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