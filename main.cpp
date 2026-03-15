#include <iostream>
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Bullet.h"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 700), "AFO 2.0");
    window.setFramerateLimit(60);


    // textures 
    sf::Texture bulletTexture;
    bulletTexture.loadFromFile("2D/BulletBlue.png");
    sf::Texture playerTexture;
    playerTexture.loadFromFile("2D/Player.png");

    Player player(playerTexture);
    std::vector<Bullet> bullets;

    // Start clock for deltatime (Makes it so speed is not FPS dependent)
    sf::Clock clock;

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
                    sf::Vector2i localPosition = sf::Mouse::getPosition(window);
                    float mouseX = localPosition.x;
                    float mouseY = localPosition.y;
                    bullets.push_back(Bullet(player.getWeaponPosition().x, player.getWeaponPosition().y, bulletTexture));
                    std::cout << "Shot fired at X: " << mouseX << " and Y: " << mouseY << "!" << std::endl;
                }
            }
        }

        player.update(deltaTime);

        for (Bullet& bullet : bullets)
        {
            bullet.update(deltaTime);
        }

        window.clear(sf::Color::Black);
        player.draw(window);

        for (Bullet& bullet : bullets)
        {
            bullet.draw(window);
        }
        window.display();
    }
}