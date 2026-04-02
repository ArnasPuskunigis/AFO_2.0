#include "Player.h"
#include <iostream>

Player::Player(sf::Texture& texture) {
    sprite.setTexture(texture);
    sprite.setScale(0.2f, 0.2f);
    health = maxHealth;
    ammo = maxAmmo;
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

void Player::checkForEnemyCollisions(std::vector<Enemy>& enemies){
    sf::FloatRect bounds = sprite.getGlobalBounds();

    for (Enemy &enemy : enemies)
    {
        if (bounds.intersects(enemy.getSprite().getGlobalBounds()))
        {
            takeDamage();
            enemy.takeDamage();
            std::cout << "An enemy has hit the player!" << std::endl;
        }
    }
}

void Player::shootBullet(){
    ammo -= 1;
    std::cout << "Ammo: " << ammo << std::endl;
}

void Player::takeDamage()
{
    if (health < 10)
    {
        health = 0;
        std::cout << "You died!";
    }
    else
    {
        health -= 10;
        std::cout << "Health: " << health << std::endl;
    }
}

sf::Sprite& Player::getSprite() {
    return sprite;
}

void Player::receiveBullets(){
    if (ammo >= maxAmmo - 5){
        ammo = maxAmmo;
    }
    else{
        ammo += 5;
    }
    std::cout << "Ammo: " << ammo << std::endl;
}

void Player::receiveHealth(){
    if (health >= maxHealth - 50){
        health = maxHealth;
    }
    else{
        health += 5;
    }
    std::cout << "Health: " << health << std::endl;
}

void Player::update(float deltaTime, std::vector<Enemy>& enemies) {
    checkForEnemyCollisions(enemies);
    handleInput(deltaTime);
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}