#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 700), "AFO 2.0");
    window.setFramerateLimit(60);

    // Start clock for deltatime (Makes it so speed is not FPS dependent)
    sf::Clock clock;

    sf::Texture texture;
    if (!texture.loadFromFile("2D/Player.png"))
    {
        std::cout << "Error loading player image" << std::endl;
    }
    sf::Sprite playerSprite;
    playerSprite.setTexture(texture);
    playerSprite.setScale(0.5f, 0.5f);

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

                    std::cout << "Shot fired at X: " << mouseX << " and Y: " << mouseY << "!" << std::endl;
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            playerSprite.move(sf::Vector2f(150, 0) * deltaTime);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            playerSprite.move(sf::Vector2f(-150, 0) * deltaTime);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
        {
            playerSprite.move(sf::Vector2f(0, -150) * deltaTime);
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
        {
            playerSprite.move(sf::Vector2f(0, 150) * deltaTime);
        }

        window.clear(sf::Color::Black);
        // draw here
        window.draw(playerSprite);

        window.display();
    }
}