#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Pickup.h"
#include "Bullet.h"
#include <math.h>
#include "AudioManager.h"

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

	// audio manager
	AudioManager audioManager;
	audioManager.load("playerLaser", "Audio/laserSmall_001.ogg");
	audioManager.load("playerThruster", "Audio/thrusterFire_003.ogg");
	audioManager.play("playerThruster", 20, true);
	audioManager.load("pickup", "Audio/lowThreeTone.ogg");
	audioManager.load("enemyExplosion", "Audio/explosionCrunch_000.ogg");
    audioManager.load("playerExplosion", "Audio/explosionCrunch_001.ogg");


	// game objects
    Player player(playerTexture);
    std::vector<Bullet> bullets;

    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Pickup>> pickups;

    // clock for deltatime
    sf::Clock clock;

    enemies.push_back(std::make_unique<Enemy>(600.0f, 600.0f, enemyTexture, audioManager));
    pickups.push_back(std::make_unique<Pickup>(10.0f, 600.0f, ammoTexture, PickupType::Ammo, audioManager));
    pickups.push_back(std::make_unique<Pickup>(600.0f, 10.0f, healthTexture, PickupType::Health, audioManager));

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

                        
						audioManager.play("playerLaser", 10);
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

        for (auto& enemy : enemies)
            enemy->update(deltaTime, bullets);

        bullets.erase(
            std::remove_if(bullets.begin(), bullets.end(),
                           [](const Bullet &bullet)
                           { return !bullet.isAlive(); }),
            bullets.end());

        enemies.erase(
            std::remove_if(enemies.begin(), enemies.end(),
                [](const std::unique_ptr<Enemy>& enemy)
                { return !enemy->isAlive(); }),
            enemies.end());

        pickups.erase(
            std::remove_if(pickups.begin(), pickups.end(),
                [](const std::unique_ptr<Pickup>& pickup)
                { return !pickup->isAlive(); }),
            pickups.end());

        window.clear(sf::Color::Black);
        window.draw(backgroundSprite);

        for (Bullet &bullet : bullets)
            bullet.draw(window);

        for (auto& enemy : enemies)
            enemy->draw(window);

        for (auto& pickup : pickups)
            pickup->draw(window);

        player.draw(window);

        window.display();
    }
}