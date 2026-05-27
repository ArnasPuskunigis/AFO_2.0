#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Player.h"
#include "Enemy.h"
#include "Pickup.h"
#include "Bullet.h"
#include <math.h>
#include "AudioManager.h"
#include "Button.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1400, 1000), "AFO 2.0");
    window.setFramerateLimit(60);

    enum class GameState { Menu, HowToPlay, Playing };
    GameState state = GameState::Menu;

    // textures

    // ui
    sf::Texture buttonTexture;
    buttonTexture.loadFromFile("2D/ButtonTexture.png");

    sf::Texture howToPlayTexture;
    howToPlayTexture.loadFromFile("2D/HowToPlayMenu-01.png");
    sf::Sprite howToPlaySprite;
    howToPlaySprite.setTexture(howToPlayTexture);
    howToPlaySprite.setScale(0.1f, 0.1f);

    // gameplay
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

    // music
    sf::SoundBuffer musicBuffer;
    if (!musicBuffer.loadFromFile("Audio/menuMusic.ogg"))
        return -1;
    sf::Sound music;
    music.setBuffer(musicBuffer);
    music.setLoop(true);
    music.setVolume(50);
    music.play();

    sf::Font font;
    font.loadFromFile("2D/GlitchInside.otf"); // grab a free .ttf and drop it in your project folder

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("ALL FOR ONE 2.0");
    titleText.setCharacterSize(80);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(500.f, 200.f);

    // buttons
    Button playButton(500.f, 450.f, 200.f, 100.f, font, "Play", buttonTexture);
    Button htpButton(450.f, 560.f, 300.f, 100.f, font, "How To Play", buttonTexture);
    Button quitButton(500.f, 670.f, 200.f, 100.f, font, "Quit", buttonTexture);
    Button exitButton(800, 800.f, 200.f, 100.f, font, "Exit", buttonTexture);

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

        playButton.update(window);
        htpButton.update(window);
        quitButton.update(window);
        exitButton.update(window);

        while (window.pollEvent(event))
        {

            // closing the gamne with Escape
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed)
            {
                if (event.key.code == sf::Keyboard::Escape)
                    window.close();
            }
            
            // menu state mouse button clicks

            if (state == GameState::Menu)
            {
                if (playButton.isClicked(window, event))
                    state = GameState::Playing;
                if (htpButton.isClicked(window, event))
                    state = GameState::HowToPlay;
                if (quitButton.isClicked(window, event))
                    window.close();
            }

            // how to play state mouse button clicks
            if (state == GameState::HowToPlay)
            {
                if (exitButton.isClicked(window, event))
                    state = GameState::Menu;
            }

            // play state mouse button clicks
            // shooting
            if (state == GameState::Playing && event.type == sf::Event::MouseButtonPressed)
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
                }
            }
        }

        window.clear(sf::Color::Black);
        window.draw(backgroundSprite);

        if (state == GameState::Menu)
        {
            window.draw(titleText);
            playButton.draw(window);
            htpButton.draw(window);
            quitButton.draw(window);
        }
        if (state == GameState::HowToPlay)
        {
            window.draw(howToPlaySprite);
            exitButton.draw(window);
        }
        else if (state == GameState::Playing)
        {
            // all your existing update logic
            player.update(deltaTime, enemies);
            for (Bullet& bullet : bullets) bullet.update(deltaTime);
            for (auto& enemy : enemies) enemy->update(deltaTime, bullets);
            for (auto& pickup : pickups) pickup->update(deltaTime, player);

            bullets.erase(std::remove_if(bullets.begin(), bullets.end(),
                [](const Bullet& b) { return !b.isAlive(); }), bullets.end());
            enemies.erase(std::remove_if(enemies.begin(), enemies.end(),
                [](const std::unique_ptr<Enemy>& e) { return !e->isAlive(); }), enemies.end());
            pickups.erase(std::remove_if(pickups.begin(), pickups.end(),
                [](const std::unique_ptr<Pickup>& p) { return !p->isAlive(); }), pickups.end());

            for (Bullet& bullet : bullets) bullet.draw(window);
            for (auto& enemy : enemies) enemy->draw(window);
            for (auto& pickup : pickups) pickup->draw(window);
            player.draw(window);
        }

        window.display();
    }
}