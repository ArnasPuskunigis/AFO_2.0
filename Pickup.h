#pragma once

#include <SFML/Graphics.hpp>

enum class PickupType {
    Health,
    Ammo
};

class Pickup {
public:
    Pickup(float x, float y, sf::Texture &texture, PickupType type);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    void kill();
    bool isAlive() const;

private:
    sf::Sprite sprite;
    bool alive =  true;
};