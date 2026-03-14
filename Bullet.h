#include <SFML/Graphics.hpp>

class Bullet {
public:
    Bullet(float x, float y, sf::Texture& texture);
    void flyForwards(float deltaTime);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::Sprite sprite;
    float speed = 100.0f;
};