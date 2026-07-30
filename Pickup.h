#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "AudioManager.h"


enum class PickupType {
    Health,
    Ammo
};

class Pickup {
public:
    Pickup(float x, float y, sf::Texture& texture, PickupType type, AudioManager& audioManager);
    void update(float deltaTime, Player& player);
    void checkForPlayerCollision(Player& player);
    void draw(sf::RenderWindow& window);
    void kill();
    bool isAlive() const;

private:
    sf::Sprite sprite;
    bool alive = true;
    float lifetime = 20.f;
    float timer = 0.f;
    PickupType pickupType;
    AudioManager& audio;
};