// Button.h
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

class Button
{
public:
    Button(float x, float y, float width, float height, sf::Font& font, std::string text, sf::Texture texture)
    {
        this->texture = texture;
        sprite.setTexture(this->texture);
        sprite.setPosition(x, y);
        sprite.setScale(width / sprite.getLocalBounds().width, height / sprite.getLocalBounds().height); // stretch to fit
        box.setPosition(x, y);
        box.setSize({ width, height });
        box.setFillColor(sf::Color(50, 50, 50));
        box.setOutlineColor(sf::Color::White);
        box.setOutlineThickness(2);

        label.setFont(font);
        label.setString(text);
        label.setCharacterSize(30);
        // centre the text inside the box
        label.setPosition(
            x + (width - label.getLocalBounds().width) / 2,
            y + ((height - label.getLocalBounds().height) / 2) - 10
        );
    }

    void update(sf::RenderWindow& window)
    {
        sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (box.getGlobalBounds().contains(mouse))
            box.setFillColor(sf::Color(100, 100, 100)); // hover colour
        else
            box.setFillColor(sf::Color(50, 50, 50));
    }

    bool isClicked(sf::RenderWindow& window, sf::Event& event)
    {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
        {
            sf::Vector2f mouse = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            return box.getGlobalBounds().contains(mouse);
        }
        return false;
    }

    void draw(sf::RenderWindow& window)
    {
        window.draw(sprite);
        window.draw(label);
    }

private:
    sf::RectangleShape box;
    sf::Text label;
    sf::Sprite sprite;
    sf::Texture texture;
};