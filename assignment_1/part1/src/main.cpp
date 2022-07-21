#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
int main()
{
    sf::RenderWindow window(sf::VideoMode(930, 625), "my window", sf::Style::Close | sf::Style::Titlebar);
    window.setFramerateLimit(60);

    sf::Texture texture;
    texture.loadFromFile("boid-sm.png");
    sf::Sprite sprite(texture);

    sf::Vector2f spritePosition(0, 0);
    sprite.setPosition(spritePosition);

    float xVelocity = 2;
    float yVelocity = 0;

    while (window.isOpen()) {
        sf::Event evnt;
        while (window.pollEvent(evnt)) {
            if (evnt.type == evnt.Closed) {
                window.close();
            }
        }
        if (spritePosition.x > 930)
        {
            spritePosition.x = 0;
        }
        spritePosition.x += xVelocity;
        spritePosition.y += yVelocity;
        sprite.setPosition(spritePosition);

        window.clear(sf::Color::White);
        window.draw(sprite);
        window.display();
    }

    return 0;
}