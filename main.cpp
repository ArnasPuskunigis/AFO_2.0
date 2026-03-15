#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Bullet.h"
#include <math.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 700), "AFO 2.0");
    window.setFramerateLimit(60);

    // textures
    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("2D/SpaceBackground4K.png");

    sf::Sprite backgroundSprite;
    backgroundSprite.setTexture(backgroundTexture);

    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("2D/BulletBlue.png");
    sf::Texture playerTexture;
    playerTexture.loadFromFile("2D/Player.png");
    sf::Texture enemyTexture;
    enemyTexture.loadFromFile("2D/Enemy.png");

    Player player(playerTexture);
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;

    // Start clock for deltatime (Makes it so speed is not FPS dependent)
    sf::Clock clock;


    //Spawn a single enemy
    enemies.push_back(Enemy(600.0f, 600.0f, enemyTexture));

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();

        sf::Event event;
        while (window.pollEvent(event))
        {

            if (event.type == sf::Event::Closed)
            {
                window.close();
                std::cout << "Game was closed!" << std::endl;
            }

            if (event.type == sf::Event::KeyPressed)
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f spawnPos = player.getWeaponPosition();
                    sf::Vector2i mouseRaw = sf::Mouse::getPosition(window);
                    sf::Vector2f mousePos(mouseRaw.x, mouseRaw.y);
                    sf::Vector2f direction = mousePos - spawnPos;
                    float angle = std::atan2(direction.y, direction.x) * (180.f / 3.14159f);
                    bullets.push_back(Bullet(spawnPos.x, spawnPos.y, angle, bulletTexture));
                }
            }
        }
        player.update(deltaTime);

        for (Bullet &bullet : bullets)
        {
            bullet.update(deltaTime);
        }

        window.clear(sf::Color::Black);
        // Always draw background first
        window.draw(backgroundSprite);

        // Draw bullets under player sprite
        for (Bullet &bullet : bullets)
        {
            bullet.draw(window);
        }

        for (Enemy &enemy : enemies)
        {
            enemy.draw(window);
        }
        
        player.draw(window);
        window.display();
    }
}