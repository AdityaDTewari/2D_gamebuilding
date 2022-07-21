#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <vector>

struct Kinematic
{
    sf::Vector2f position;
    float orientation;
    sf::Vector2f velocity;
    float rotation;
    sf::Vector2f linear;
    float angular;
};

void update(Kinematic &obj)
{
    obj.position += obj.velocity;
    obj.orientation += obj.rotation;

    obj.velocity += obj.linear;
    obj.rotation += obj.angular;
}


float getLength(sf::Vector2f vec)
{
    float dem = sqrt(pow(vec.x, 2) + pow(vec.y, 2));
    return dem;
}

float newOrientation(float current, sf::Vector2f velocity)
{
    if (getLength(velocity) > 0)
    {
        return std::atan2(velocity.y, velocity.x);
    }
    else
    {
        return current;
    }
}

sf::Vector2f normalize(sf::Vector2f source)
{
    float length = sqrt((source.x * source.x) + (source.y * source.y));
    if (length != 0)
        return sf::Vector2f(source.x / length, source.y / length);
    else
        return source;
}

float mapToRange(float rotation)
{
    rotation = fmod(rotation, 2 * M_PI);
    if (rotation <= M_PI)
    {
        return rotation;
    }
    else if (rotation > M_PI)
    {
        return rotation - M_PI;
    }
    else
    {
        return rotation + M_PI;
    }
}

void KinematicSeek(Kinematic &ch, Kinematic &tar, float max_speed)
{
    Kinematic result;
    result.velocity = tar.position - ch.position;

    result.velocity = normalize(result.velocity);
    result.velocity *= max_speed;

    tar.orientation = newOrientation(ch.orientation, result.velocity);
    result.rotation = tar.orientation - ch.orientation;
    result.rotation /= 4;

    //result.rotation = 0;
    //std::cout << tar.orientation << " " << ch.orientation << " " << ch.rotation << std::endl;
    ch.velocity = result.velocity;
    //std::cout << ch.velocity.x << " " << ch.velocity.y << std::endl;
    ch.rotation = result.rotation;  
}

void arrive(Kinematic &ch, Kinematic &tar, float goal_speed, float max_speed, float radius_satis, float radius_dacc, float goal_rotation, float max_rotation)
{
    sf::Vector2f direction = tar.position - ch.position;
    float frames_to_orient = 30;
    float distance = getLength(direction);
    if (distance < radius_satis)
            {
                goal_speed = 0;
            }
            else if (distance > radius_dacc)
            {
                goal_speed = max_speed;
            }
            else
            {
                goal_speed = max_speed * distance / radius_dacc;
            }
    Kinematic result;
    result.velocity = direction;
    result.velocity = normalize(result.velocity);
    result.velocity *= goal_speed;
    result.linear = result.velocity - ch.velocity;
    ch.velocity = result.velocity;
    ch.linear = result.linear;

    tar.orientation = newOrientation(ch.orientation, result.velocity);
    result.rotation = (tar.orientation - ch.orientation) / frames_to_orient;
    result.rotation = mapToRange(result.rotation);
    //result.rotation = (tar.orientation - ch.orientation);
    result.angular =  result.rotation - ch.rotation;
    ch.rotation = result.rotation;
    goal_rotation = 0;
    //std::cout << tar.orientation << " " << ch.orientation << " "<< result.rotation << " " << goal_rotation << std::endl;
}
int main()
{
    sf::RenderWindow window(sf::VideoMode(930, 625), "my window", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture texture;
    texture.loadFromFile("boid-sm.png");
    sf::Sprite sprite(texture);

    Kinematic ch, tar;
    //SteeringOutput ch_so;

    ch.position = sf::Vector2f(0, 0);
    ch.velocity = sf::Vector2f(0, 0);
    ch.orientation = 0;
    ch.rotation = 0;
    ch.linear = sf::Vector2f(0, 0);
    ch.angular = 0;
    //tar.position = sf::Vector2f(100, 200);

    //marker
    sf::Sprite marker(texture);
    marker.setPosition(sf::Vector2f(300, 300));

    float radius_dacc = 200;
    float radius_satis = 50;
    sf::Vector2i mouse_position(300, 300);
    tar.position = sf::Vector2f(mouse_position);
    float goal_speed = 0;
    float goal_rotation = 0;
    float max_rotation = 0.25;

    sf::Clock clock;
    sf::Time time_d = sf::seconds(0.02);

    //breadcrums
    std::vector<sf::CircleShape> circles;
    std::vector<sf::Vector2f> c_pos;

    while (window.isOpen()) {
        sf::Event evnt;
        while (window.pollEvent(evnt)) {
            if (evnt.type == evnt.Closed) {
                window.close();
            }

            if (evnt.type == sf::Event::MouseButtonReleased)
            {
                mouse_position = sf::Mouse::getPosition(window);
                tar.position = sf::Vector2f(mouse_position);
                marker.setPosition(tar.position);
            }
        }
        sf::Time elapsed = clock.getElapsedTime();
        if (elapsed >= time_d)
        {
            sprite.setPosition(ch.position);

            float temp = ch.orientation * 180 / M_PI;

            sprite.setRotation(temp);
            //sprite.rotate(temp);

            //KinematicSeek(ch, tar, 2);
            arrive(ch, tar, goal_speed, 4, radius_satis, radius_dacc, goal_rotation, max_rotation);
            update(ch);
            std::cout << ch.velocity.y << " " << ch.velocity.x << std::endl;
            if (ch.velocity != sf::Vector2f(0, 0))
            {
                sf::CircleShape crum;
                crum.setRadius(1);
                crum.setOutlineColor(sf::Color::Black);
                crum.setFillColor(sf::Color::Black);
                circles.push_back(crum);
                c_pos.push_back(ch.position);
            }
            window.clear(sf::Color::White);
            window.draw(sprite);
            //window.draw(marker);

            for (int i = 0; i < circles.size(); i++)
            {
                circles[i].setPosition(c_pos[i]);
                window.draw(circles[i]);
            }

            window.display();
            clock.restart();
        }
        
    }
    return 0;
}