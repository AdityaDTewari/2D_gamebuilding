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

    sf::Sprite sprite2(texture);
    sf::Sprite sprite3(texture);
    sf::Sprite sprite4(texture);

    sf::Vector2f spritePosition(0, 0);
    sprite.setPosition(spritePosition);

    sf::Vector2f spritePosition2(0, 0);
    sprite2.setPosition(spritePosition2);
    sf::Vector2f spritePosition3(0, 0);
    sprite2.setPosition(spritePosition3);
    sf::Vector2f spritePosition4(0, 0);
    sprite2.setPosition(spritePosition4);

    float xVelocity_1 = 2.976;
    float yVelocity_1 = 0;

    float xVelocity_2 = 2.976;
    float yVelocity_2 = 0;
    float xVelocity_3 = 2.976;
    float yVelocity_3 = 0;
    float xVelocity_4 = 2.976;
    float yVelocity_4 = 0;

    bool show1 = true;
    bool show2 = false;
    bool show3 = false;
    bool show4 = false;

    while (window.isOpen()) {
        sf::Event evnt;
        while (window.pollEvent(evnt)) {
            if (evnt.type == evnt.Closed) {
                window.close();
            }
        }

        //sprite1
        if (spritePosition.x >= 930 && spritePosition.y == 0)
        {
            show2 = true;
            sprite.rotate(90.f);
            xVelocity_1 = 0;
            yVelocity_1 = 2;
        }
        if (spritePosition.y >= 625 && spritePosition.x >= 930)
        {
            sprite.rotate(90.f);
            xVelocity_1 = -2.976;
            yVelocity_1 = 0;
        }
        if (spritePosition.x <= 0 && spritePosition.y >= 625)
        {
            sprite.rotate(90.f);
            xVelocity_1 = 0;
            yVelocity_1 = -2;
        }
        if (spritePosition.x < 0 && spritePosition.y < 0)
        {
            spritePosition = sf::Vector2f(0, 0);
            sprite.setPosition(spritePosition);
            sprite.rotate(90.f);
            xVelocity_1 = 2.976;
            yVelocity_1 = 0;

            show1 = false;
        }
        
        //sprite2
        if (spritePosition2.x >= 930 && spritePosition2.y == 0)
        {
            show3 = true;
            sprite2.rotate(90.f);
            xVelocity_2 = 0;
            yVelocity_2 = 2;
        }
        if (spritePosition2.y >= 625 && spritePosition2.x >= 930)
        {
            sprite2.rotate(90.f);
            xVelocity_2 = -2.976;
            yVelocity_2 = 0;
        }
        if (spritePosition2.x <= 0 && spritePosition2.y >= 625)
        {
            sprite2.rotate(90.f);
            xVelocity_2 = 0;
            yVelocity_2 = -2;
        }
        if (spritePosition2.x < 0 && spritePosition2.y < 0)
        {
            spritePosition2 = sf::Vector2f(0, 0);
            sprite2.setPosition(spritePosition2);
            sprite2.rotate(90.f);
            xVelocity_2 = 2.976;
            yVelocity_2 = 0;
            show2 = false;
        }

        //sprite3
        if (spritePosition3.x >= 930 && spritePosition3.y == 0)
        {
            show4 = true;
            sprite3.rotate(90.f);
            xVelocity_3 = 0;
            yVelocity_3 = 2;
        }
        if (spritePosition3.y >= 625 && spritePosition3.x >= 930)
        {
            sprite3.rotate(90.f);
            xVelocity_3 = -2.976;
            yVelocity_3 = 0;
        }
        if (spritePosition3.x <= 0 && spritePosition3.y >= 625)
        {
            sprite3.rotate(90.f);
            xVelocity_3 = 0;
            yVelocity_3 = -2;
        }
        if (spritePosition3.x < 0 && spritePosition3.y < 0)
        {
            spritePosition3 = sf::Vector2f(0, 0);
            sprite3.setPosition(spritePosition3);
            sprite3.rotate(90.f);
            xVelocity_3 = 2.976;
            yVelocity_3 = 0;
            show3 = false;
        }

        //sprite4
        if (spritePosition4.x >= 930 && spritePosition4.y == 0)
        {
            sprite4.rotate(90.f);
            xVelocity_4 = 0;
            yVelocity_4 = 2;
        }
        if (spritePosition4.y >= 625 && spritePosition4.x >= 930)
        {
            sprite4.rotate(90.f);
            xVelocity_4 = -2.976;
            yVelocity_4 = 0;
        }
        if (spritePosition4.x <= 0 && spritePosition4.y >= 625)
        {
            sprite4.rotate(90.f);
            xVelocity_4 = 0;
            yVelocity_4 = -2;
        }
        if (spritePosition4.x < 0 && spritePosition4.y < 0)
        {
            spritePosition4 = sf::Vector2f(0, 0);
            sprite4.setPosition(spritePosition4);
            sprite4.rotate(90.f);
            xVelocity_4 = 2.976;
            yVelocity_4 = 0;
            show4 = false;
            show1 = true;

        }

        window.clear(sf::Color::White);

        if (show1)
        {
            spritePosition.x += xVelocity_1;
            spritePosition.y += yVelocity_1;
            sprite.setPosition(spritePosition);
            window.draw(sprite);
        }

        

        if (show2)
        {
            spritePosition2.x += xVelocity_2;
            spritePosition2.y += yVelocity_2;
            sprite2.setPosition(spritePosition2);
            window.draw(sprite2);
        }
        if (show3)
        {
            spritePosition3.x += xVelocity_3;
            spritePosition3.y += yVelocity_3;
            sprite3.setPosition(spritePosition3);
            window.draw(sprite3);
        }
        if (show4)
        {
            spritePosition4.x += xVelocity_4;
            spritePosition4.y += yVelocity_4;
            sprite4.setPosition(spritePosition4);
            window.draw(sprite4);
        }

        window.display();
    }

    return 0;
}