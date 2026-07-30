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
#include "EventSystem.h"

enum anchorEnum
{
    Middle, TopLeft, TopRight, BottomLeft, BottomRight
};

sf::Vector2f calculateAnchor(float width, float height, sf::RenderWindow& window, anchorEnum anchor)
{
    float w = window.getSize().x;
    float h = window.getSize().y;

    switch (anchor)
    {
    case anchorEnum::Middle:
        return sf::Vector2f((w / 2) - (width / 2), (h / 2) - (height / 2));
    case anchorEnum::TopLeft:
        return sf::Vector2f(0, 0);
    case anchorEnum::TopRight:
        return sf::Vector2f(w - width, 0);
    case anchorEnum::BottomLeft:
        return sf::Vector2f(0, h - height);
    case anchorEnum::BottomRight:
        return sf::Vector2f(w - width, h - height);
    default:
        return sf::Vector2f(0, 0);
    }
}

void applyLetterbox(sf::RenderWindow& window, sf::View& view, float virtualWidth, float virtualHeight)
{
    float windowRatio = window.getSize().x / (float)window.getSize().y;
    float viewRatio = virtualWidth / virtualHeight;

    float sizeX = 1.f, sizeY = 1.f;
    float posX = 0.f, posY = 0.f;

    if (windowRatio >= viewRatio)
    {
        sizeX = viewRatio / windowRatio;
        posX = (1.f - sizeX) / 2.f;
    }
    else
    {
        sizeY = windowRatio / viewRatio;
        posY = (1.f - sizeY) / 2.f;
    }

    view.setViewport(sf::FloatRect(posX, posY, sizeX, sizeY));
    window.setView(view);
}

sf::Vector2f getSpawnPosition(sf::Vector2f playerPos, float minDist, float maxDist)
{
    float angle = (rand() % 360) * (3.14159f / 180.f);
    float dist = minDist + (rand() % (int)(maxDist - minDist));
    return sf::Vector2f(
        playerPos.x + std::cos(angle) * dist,
        playerPos.y + std::sin(angle) * dist
    );
}

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "AFO 2.0");
    window.setFramerateLimit(60);

    const float VIRTUAL_W = 1920.f;
    const float VIRTUAL_H = 1080.f;

    EventSystem events;
    int playerScore = 0;

    sf::View view(sf::FloatRect(0, 0, VIRTUAL_W, VIRTUAL_H));
    sf::View camera(sf::FloatRect(0, 0, VIRTUAL_W, VIRTUAL_H));
    applyLetterbox(window, view, VIRTUAL_W, VIRTUAL_H);

    enum class GameState {
        Menu, 
        HowToPlay, 
        Playing, 
        Paused
    };
    GameState state = GameState::Menu;

    // base anchor
    sf::Vector2f anchor;
    sf::FloatRect bounds;
    sf::Vector2f pos;

    // font
    sf::Font font;
    font.loadFromFile("2D/GlitchInside.otf");

    // game textures

    sf::Texture backgroundTexture;
    backgroundTexture.loadFromFile("2D/SpaceBackground4K.png");
    sf::Sprite backgroundSprite;
    backgroundTexture.setRepeated(true);
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

    // game objects
    Player player(playerTexture, events);
    std::vector<Bullet> bullets;

    std::vector<std::unique_ptr<Enemy>> enemies;
    std::vector<std::unique_ptr<Pickup>> pickups;

    // enemy spawner

    float spawnTimer = 0.f;
    float spawnInterval = 3.f;
    int wave = 1;
    int enemiesPerWave = 3;
    int enemiesSpawned = 0;

    // pickup spawner

    float pickupSpawnTimer = 0.f;
    float pickupSpawnInterval = 4.f;
    int maxPickups = 5;

    // ui + textures

    sf::Text titleText;
    titleText.setFont(font);
    titleText.setString("ALL FOR ONE 2.0");
    titleText.setCharacterSize(120);
    titleText.setFillColor(sf::Color::White);

    bounds = titleText.getLocalBounds();
    pos = calculateAnchor(bounds.width, bounds.height, window, Middle);
    titleText.setPosition(pos.x, 200);

    sf::Texture buttonTexture;
    buttonTexture.loadFromFile("2D/ButtonTexture.png");

    sf::Texture howToPlayTexture;
    howToPlayTexture.loadFromFile("2D/HowToPlayMenu-01.png");
    sf::Sprite howToPlaySprite;
    howToPlaySprite.setTexture(howToPlayTexture);
    howToPlaySprite.setScale(0.15f, 0.15f);

    // buttons
    anchor = calculateAnchor(200, 100, window, Middle);
    Button playButton(anchor.x, 450.f, 200.f, 100.f, font, "Play", buttonTexture);
    Button quitButton(anchor.x, 670.f, 200.f, 100.f, font, "Quit", buttonTexture);

    anchor = calculateAnchor(300, 100, window, Middle);
    Button htpButton(anchor.x, 560.f, 300.f, 100.f, font, "How To Play", buttonTexture);
    anchor = calculateAnchor(200, 100, window, BottomRight);
    Button exitButton(anchor.x, anchor.y, 200.f, 100.f, font, "Exit", buttonTexture);

    // HUD
    sf::Texture pauseButtonTexture;
    pauseButtonTexture.loadFromFile("2D/PauseButton.png");

    anchor = calculateAnchor(100, 100, window, TopRight);
    Button pauseButton(anchor.x, anchor.y, 100.f, 100.f, font, "", pauseButtonTexture);

    sf::Texture scoreTexture;
    scoreTexture.loadFromFile("2D/BorderForIcons.png");
    sf::Sprite scoreSprite;
    scoreSprite.setTexture(scoreTexture);
    anchor = calculateAnchor(200, 130, window, BottomRight);
    scoreSprite.setPosition(anchor);
    scoreSprite.setScale(0.05f, 0.05f);

    sf::Text scoreText;
    scoreText.setFont(font);
    scoreText.setString(std::to_string(player.getScore()));
    scoreText.setCharacterSize(60);
    scoreText.setFillColor(sf::Color::Black);

    pos = calculateAnchor(120, 110, window, BottomRight);
    scoreText.setPosition(pos.x, pos.y);

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

    // clock for deltatime
    sf::Clock clock;

    //enemies.push_back(std::make_unique<Enemy>(600.0f, 600.0f, enemyTexture, audioManager, events));
    //pickups.push_back(std::make_unique<Pickup>(10.0f, 600.0f, ammoTexture, PickupType::Ammo, audioManager));
    //pickups.push_back(std::make_unique<Pickup>(600.0f, 10.0f, healthTexture, PickupType::Health, audioManager));

    while (window.isOpen())
    {
        float deltaTime = clock.restart().asSeconds();
        sf::Event event;

        playButton.update(window);
        htpButton.update(window);
        quitButton.update(window);
        exitButton.update(window);
        pauseButton.update(window);


        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Resized)
                applyLetterbox(window, view, VIRTUAL_W, VIRTUAL_H);

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
            if (state == GameState::Playing && event.type == sf::Event::MouseButtonPressed)
            {
                // shooting
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    if (player.getAmmo() >= 1)
                    {
                        player.shootBullet();
                        sf::Vector2f spawnPos = player.getWeaponPosition();
                        sf::Vector2i mouseRaw = sf::Mouse::getPosition(window);
                        sf::Vector2f mousePos = window.mapPixelToCoords(mouseRaw, camera);
                        sf::Vector2f direction = mousePos - spawnPos;
                        float angle = std::atan2(direction.y, direction.x) * (180.f / 3.14159f);
                        bullets.push_back(Bullet(spawnPos.x, spawnPos.y, angle, bulletTexture));
                        audioManager.play("playerLaser", 10);
                    }
                }

                // pause
                if (pauseButton.isClicked(window, event) || event.KeyPressed == sf::Keyboard::P)
                {
                    std::cout << "Paused" << std::endl;
                    //state = GameState::Paused;
                }
            }
        }

        window.clear(sf::Color::Black);
        window.setView(camera);
        window.draw(backgroundSprite);

        if (state == GameState::Menu)
        {
            window.setView(view);
            window.draw(titleText);
            playButton.draw(window);
            htpButton.draw(window);
            quitButton.draw(window);
        }
        if (state == GameState::HowToPlay)
        {
            window.setView(view);
            window.draw(howToPlaySprite);
            exitButton.draw(window);
        }
        else if (state == GameState::Playing)
        {
            // Scrolling Background
            window.setView(camera);
            float parallaxSpeed = 0.2f;
            sf::Vector2f camPos = camera.getCenter();
            backgroundSprite.setTextureRect(sf::IntRect(
                (int)(camPos.x * parallaxSpeed),
                (int)(camPos.y * parallaxSpeed),
                (int)VIRTUAL_W,
                (int)VIRTUAL_H
            ));
            backgroundSprite.setPosition(camPos.x - VIRTUAL_W / 2, camPos.y - VIRTUAL_H / 2);
            window.draw(backgroundSprite);

            player.update(deltaTime, enemies);
            sf::Vector2f playerPos = player.getSprite().getPosition();

            // spawning enemies
            events.emit(GameEvent::WaveStarted);
            spawnTimer += deltaTime;
            if (spawnTimer >= spawnInterval && enemiesSpawned < enemiesPerWave)
            {
                spawnTimer = 0.f;
                sf::Vector2f spawnPos = getSpawnPosition(player.getSprite().getPosition(), 1000.f, 1500.f);
                enemies.push_back(std::make_unique<Enemy>(spawnPos.x, spawnPos.y, enemyTexture, audioManager, events));
                enemiesSpawned++;
            }
            // spawning pickups

            pickupSpawnTimer += deltaTime;
            if (pickupSpawnTimer >= pickupSpawnInterval && (int)pickups.size() < maxPickups)
            {
                pickupSpawnTimer = 0.f;
                sf::Vector2f spawnPos = getSpawnPosition(player.getSprite().getPosition(), 800.f, 1200.f);

                // randomly pick health or ammo
                if (rand() % 2 == 0)
                    pickups.push_back(std::make_unique<Pickup>(spawnPos.x, spawnPos.y, ammoTexture, PickupType::Ammo, audioManager));
                else
                    pickups.push_back(std::make_unique<Pickup>(spawnPos.x, spawnPos.y, healthTexture, PickupType::Health, audioManager));
            }

            // start next wave when all enemies are dead
            if (enemies.empty() && enemiesSpawned >= enemiesPerWave)
            {
                wave++;
                enemiesPerWave += 2; // each wave gets harder
                spawnInterval = std::max(1.f, spawnInterval - 0.2f);
                enemiesSpawned = 0;
                spawnTimer = 0.f;
                std::cout << "Wave " << wave << " starting!" << std::endl;
            }

            for (Bullet& bullet : bullets) bullet.update(deltaTime);
            for (auto& enemy : enemies) enemy->update(deltaTime, bullets, playerPos);
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
            
            // Camera follows player
            player.draw(window);
            sf::Vector2f cameraPos = camera.getCenter();
            float lerpSpeed = 5.0f;
            sf::Vector2f newPos = cameraPos + (playerPos - cameraPos) * lerpSpeed * deltaTime;
            camera.setCenter(newPos);

            // HUD
            window.setView(view);
            scoreText.setString(std::to_string(player.getScore()));
            pauseButton.draw(window);
            window.draw(scoreSprite);
            window.draw(scoreText);
        }

        window.display();
    }

}