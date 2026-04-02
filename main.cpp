#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Pickup.h"
#include "Bullet.h"
#include <math.h>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1400, 1000), "AFO 2.0");
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
    sf::Texture ammoTexture;
    ammoTexture.loadFromFile("2D/AmmoCrate.png");
    sf::Texture healthTexture;
    healthTexture.loadFromFile("2D/HealthBox.png");

    // sound buffers
    sf::SoundBuffer playerLaserBuffer;
    if (!playerLaserBuffer.loadFromFile("Audio/laserSmall_001.ogg"))
        return -1;

    sf::SoundBuffer playerThrusterBuffer;
    if (!playerThrusterBuffer.loadFromFile("Audio/thrusterFire_003.ogg"))
        return -1;

    sf::SoundBuffer pickupBuffer;
    if (!pickupBuffer.loadFromFile("Audio/lowThreeTone.ogg"))
        return -1;

    sf::Sound playerLaserSound;
    playerLaserSound.setBuffer(playerLaserBuffer);
    playerLaserSound.setVolume(10);

    sf::Sound playerThrusterSound;
    playerThrusterSound.setBuffer(playerThrusterBuffer);
    playerThrusterSound.setVolume(5);
    playerThrusterSound.setLoop(true);

    sf::Sound pickupSound;
    pickupSound.setBuffer(pickupBuffer);
    pickupSound.setVolume(10);

    Player player(playerTexture);
    std::vector<Enemy> enemies;
    std::vector<Bullet> bullets;
    std::vector<Pickup> pickups;

    // Start clock for deltatime (Makes it so speed is not FPS dependent)
    sf::Clock clock;

    // Spawn a single enemy
    enemies.push_back(Enemy(600.0f, 600.0f, enemyTexture));
    pickups.push_back(Pickup(10.0f, 600.0f, ammoTexture, PickupType::Ammo));
    pickups.push_back(Pickup(600.0f, 10.0f, healthTexture, PickupType::Health));
    playerThrusterSound.play();

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
                    if (player.getAmmo() >= 1)
                    {
                        player.shootBullet();
                        sf::Vector2f spawnPos = player.getWeaponPosition();
                        sf::Vector2i mouseRaw = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePos(mouseRaw.x, mouseRaw.y);
                        sf::Vector2f direction = mousePos - spawnPos;
                        float angle = std::atan2(direction.y, direction.x) * (180.f / 3.14159f);
                        bullets.push_back(Bullet(spawnPos.x, spawnPos.y, angle, bulletTexture));

                        
                        playerLaserSound.play();
                    }
                    else{
                        std::cout << "You have no ammo left! Pick up some of those blue ammo crates" << std::endl;
                    }
                }
            }
        }

        player.update(deltaTime, enemies);

        for (Bullet &bullet : bullets)
            bullet.update(deltaTime);

        for (Enemy &enemy : enemies)
            enemy.update(deltaTime, bullets);

        for (Pickup& pickup : pickups)
            pickup.update(deltaTime, player);

        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                           [](const Bullet &bullet)
                           { return !bullet.isAlive(); }),
            bullets.end());

        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                           [](const Enemy &enemy)
                           { return !enemy.isAlive(); }),
            enemies.end());

        pickups.erase(
            std::remove_if(pickups.begin(), pickups.end(),
                [](const Pickup& pickup)
                { return !pickup.isAlive(); }),
            pickups.end());

        window.clear(sf::Color::Black);
        window.draw(backgroundSprite);

        for (Bullet &bullet : bullets)
            bullet.draw(window);

        for (Enemy &enemy : enemies)
            enemy.draw(window);

        for (Pickup &pickup : pickups)
            pickup.draw(window);

        player.draw(window);

        window.display();
    }
}