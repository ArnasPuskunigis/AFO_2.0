#include <SFML/Graphics.hpp>

class Enemy {
public:
    Enemy(float x, float y, sf::Texture& texture);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite sprite;
};