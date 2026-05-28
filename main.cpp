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

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "AFO 2.0");
    window.setFramerateLimit(60);

    const float VIRTUAL_W = 1920.f;
    const float VIRTUAL_H = 1080.f;

    sf::View view(sf::FloatRect(0, 0, VIRTUAL_W, VIRTUAL_H));
    applyLetterbox(window, view, VIRTUAL_W, VIRTUAL_H);

    enum class GameState { Menu, HowToPlay, Playing };
    GameState state = GameState::Menu;

    // base anchor
    sf::Vector2f anchor;

    // textures

    // ui
    sf::Texture buttonTexture;
    buttonTexture.loadFromFile("2D/ButtonTexture.png");

    sf::Texture howToPlayTexture;
    howToPlayTexture.loadFromFile("2D/HowToPlayMenu-01.png");
    sf::Sprite howToPlaySprite;
    howToPlaySprite.setTexture(howToPlayTexture);
    howToPlaySprite.setScale(0.15f, 0.15f);

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
    titleText.setCharacterSize(120);
    titleText.setFillColor(sf::Color::White);

    sf::FloatRect bounds = titleText.getLocalBounds();
    sf::Vector2f pos = calculateAnchor(bounds.width, bounds.height, window, Middle);
    titleText.setPosition(pos.x, 200);



    // buttons
    anchor = calculateAnchor(200, 100, window, Middle);
    Button playButton(anchor.x, 450.f, 200.f, 100.f, font, "Play", buttonTexture);
    Button quitButton(anchor.x, 670.f, 200.f, 100.f, font, "Quit", buttonTexture);
    
    anchor = calculateAnchor(300, 100, window, Middle);
    Button htpButton(anchor.x, 560.f, 300.f, 100.f, font, "How To Play", buttonTexture);
    anchor = calculateAnchor(200, 100, window, BottomRight);
    Button exitButton(anchor.x, anchor.y, 200.f, 100.f, font, "Exit", buttonTexture);

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

        window.clear(sf::Color::Black); // bars stay black
        window.setView(view);
        window.draw(backgroundSprite);

        //window.clear(sf::Color::Black);
        //window.draw(backgroundSprite);

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