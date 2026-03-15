#include <SFML/Graphics.hpp>

class Player {
public:
    Player(sf::Texture& texture);
    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);
    sf::Vector2i getWeaponPosition();

private:
    sf::Sprite sprite;
    float speed = 150.0f;
};