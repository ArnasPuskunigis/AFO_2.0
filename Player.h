#include <SFML/Graphics.hpp>

class Player {
public:
    Player();
    void handleInput(float deltaTime);
    void update(float deltaTime);
    void draw(sf::RenderWindow& window);

private:
    sf::Texture texture;
    sf::Sprite sprite;
    float speed = 150.0f;
};