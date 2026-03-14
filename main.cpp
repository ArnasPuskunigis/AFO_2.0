#include <iostream>
#include <SFML/Graphics.hpp>

int main()
{
    sf::RenderWindow window(sf::VideoMode(1000, 700), "AFO 2.0");

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
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
                std::cout << "Game was closed!" << std::endl;
            }

            if (event.type == sf::Event::KeyPressed)
            {
                std::cout << event.key.code << " was pressed!" << std::endl;
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                std::cout << "Left pressed!" << std::endl;
            }
        }

        window.clear(sf::Color::Black);
        window.display();
    }
}