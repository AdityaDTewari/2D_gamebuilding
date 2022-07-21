#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <math.h>
class SteeringBehaviour
{
    public:
    sf::Vector2f position;
    sf::Vector2f velocity;
    float orientation;
    float rot;
};
class Position : public virtual SteeringBehaviour
{
    public:
    void set_default()
    {
        position = sf::Vector2f(0, 0);
    }
    void updatePos(sf::Sprite &sp)
    {
        sp.setPosition(position);
    }
};
class Orientation : public virtual SteeringBehaviour
{
    public:
    void set_default()
    {
        orientation = 0;
    }
    void updateOrien()
    {
        //
    }
};
class Velocity : public virtual SteeringBehaviour
{
    public:
    void set_default()
    {
        velocity = sf::Vector2f(0, 0);
    }
    void updateVelocity(sf::Sprite &sp)
    {
        sp.move(velocity);
    }
};
class Rotation : public virtual SteeringBehaviour
{
    public:
    void set_default()
    {
        rot = 0;
    }
    void updateRot(sf::Sprite &sp)
    {
        sp.setRotation(rot);
    }
};
class Update : public virtual Position, public virtual Orientation, public virtual Velocity, public virtual Rotation
{
    public:
    void all_default()
    {
        Position::set_default();
        Velocity::set_default();
        Orientation::set_default();
        Rotation::set_default();
    }
};

int main()
{
    sf::RenderWindow window(sf::VideoMode(930, 625), "assignment 2 part 1", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture texture;
    texture.loadFromFile("boid-sm.png");
    sf::Sprite sprite(texture);

    Update ch, tar;
    ch.all_default();
    tar.all_default();

    ch.position = sf::Vector2f(10, 10);
    ch.updatePos(sprite);

    sf::Clock clock;
    sf::Time td = sf:: seconds(0.02);
    sf::Vector2i mouse_pos_1 = sf::Mouse::getPosition(window);

    while (window.isOpen()) {

        sf::Event evnt;
        while (window.pollEvent(evnt)) {
            if (evnt.type == evnt.Closed) {
                window.close();
            }
        }
        
        sf::Time elapsed = clock.getElapsedTime();

        if (elapsed >= td) {
            sf::Vector2i mouse_pos_2 = sf::Mouse::getPosition(window);
            tar.position = sf::Vector2f(mouse_pos_2);
            ch.velocity = tar.position - sf::Vector2f(mouse_pos_1);
            ch.updateVelocity(sprite);

            tar.orientation = std::atan2(ch.velocity.y, ch.velocity.x);
            
            if (mouse_pos_1 != mouse_pos_2)
            {
                for (int i = 1; i <= 60; i++)
                {
                    float temporary_orientation = ch.orientation + i * (tar.orientation - ch.orientation) / 60;
                    ch.rot = temporary_orientation * 180.0 / M_PI;
                    ch.updateRot(sprite);
                }
                ch.rot = tar.orientation * 180.0 / M_PI;
                ch.updateRot(sprite);
                ch.orientation = tar.orientation;
            }

            mouse_pos_1 = sf::Vector2i(tar.position);

            window.clear(sf::Color::White);
            window.draw(sprite);
            window.display();
            clock.restart();
        }
    }

    return 0;
}