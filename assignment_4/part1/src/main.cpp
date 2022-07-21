#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <random>

struct Kinematic
{
    sf::Vector2f position;
    float orientation;
    sf::Vector2f velocity;
    float rotation;
    sf::Vector2f linear;
    float angular;
};

struct Node {
    std::string val;
    Node* left;
    Node* right;
};

bool condition1(Kinematic &obj)
{
    if (obj.position.x > 930 || obj.position.y > 625 || obj.position.x < 0 || obj.position.y < 0)
    {
        //std::cout << obj.position.x << " " << obj.position.y << std::endl;
        return true;
    }
    else
    {
        //std::cout << "not yet " << std::endl;
        return false;
    }
}

void action1(Kinematic &obj)
{
    obj.position = sf::Vector2f(425, 312);
    //std::cout << "Action 1" << std::endl;
}

bool condition2(Kinematic &obj, Kinematic &obj2)
{
    sf::Vector2f temp = obj2.position - obj.position;
    float length = sqrt((temp.x * temp.x) + (temp.y * temp.y));
    if (length > 1)
    {
        //std::cout << "full speed" << std::endl;
        return true;
    }
    else
    {
        //std::cout << "slowing" << std::endl;
        return false;
    }
}

void action2(Kinematic &obj, sf::Time &time, sf::Clock &clk)
{
    sf::Time time_d2 = sf::seconds(0.5);
    if (time > time_d2)
    {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> uni(-1000,1000);
        float f1 = uni(rng);
        float f2 = uni(rng);
        obj.position = sf::Vector2f(f1, f2);
        clk.restart();
    }
    //std::cout << "Action 2" << std::endl;
}

bool condition3(sf::Vector2f pos)
{
    if (pos == sf::Vector2f(0, 0))
    {
        return false;
    }
    else
    {
        return true;
    }
}

void action3(Kinematic &obj, sf::Vector2f &pos)
{
    obj.position = pos;
    pos = sf::Vector2f(0, 0);
}

void decisionTree(Kinematic &obj, Kinematic &obj2, struct Node* node, sf::Time &time, sf::Clock &clk, sf::Vector2f &pos)
{
    bool ans;
    while (node->left != NULL && node->right != NULL)
    {
        if (node->val[0] == 'C')
        {
            if (node->val == "C1")
            {
                ans = condition1(obj);
                if (ans)
                {
                    node = node->left;
                }
                else
                {
                    node = node->right;
                }
            }
            else if (node->val == "C2")
            {
                ans = condition2(obj, obj2);
                if (ans)
                {
                    node = node->left;
                }
                else
                {
                    node = node->right;
                }
            }
            else if (node->val == "C3")
            {
                ans = condition3(pos);
                if (ans)
                {
                    node = node->left;
                }
                else
                {
                    node = node->right;
                }
            }
        }
        else
        {
            if(node->val == "A1")
            {
                action1(obj2);
                break;
            }

            else if(node->val == "A2")
            {
                action2(obj2, time, clk);
                break;
            }

            else if (node->val == "A3")
            {
                action3(obj2, pos);
                break;
            }
        }
    }
}

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
    Node* one = new Node;
    Node* two = new Node;
    Node* three = new Node;
    Node* four = new Node;
    Node* five = new Node;
    Node* six = new Node;
    Node* e1 = new Node;
    Node* e2 = new Node;
    one->val = "C1";
    one->left = two;
    one->right = three;
    two->val = "A1";
    two->left = e1;
    two->right = e2;
    three->val = "C2";
    three->left = five;
    three->right = four;
    four->val = "A2";
    four->left = e1;
    four->right = e2;
    five->val = "C3";
    five->left = six;
    five->right = e2;
    six->val = "A3";
    six->left = e1;
    six->right = e2;
    e1->left = NULL;
    e1->right = NULL;
    e2->left = NULL;
    e2->right = NULL;

    sf::RenderWindow window(sf::VideoMode(930, 625), "my window", sf::Style::Close | sf::Style::Titlebar);

    sf::Texture texture;
    texture.loadFromFile("boid-sm.png");
    sf::Sprite sprite(texture);

    Kinematic ch, tar;

    ch.position = sf::Vector2f(0, 0);
    ch.velocity = sf::Vector2f(0, 0);
    ch.orientation = 0;
    ch.rotation = 0;
    ch.linear = sf::Vector2f(0, 0);
    ch.angular = 0;

    float radius_dacc = 50;
    float radius_satis = 0;
    sf::Vector2i mouse_position(0, 0);
    sf::Vector2f pos = sf::Vector2f(0, 0);
    //tar.position = sf::Vector2f(mouse_position);
    tar.position = sf::Vector2f(970, 300);
    float goal_speed = 0;
    float goal_rotation = 0;
    float max_rotation = 0.25;

    sf::Clock clock;
    sf::Time time_d = sf::seconds(0.02);
    sf::Clock clock2;

    //breadcrums
    //std::vector<sf::CircleShape> circles;
    //std::vector<sf::Vector2f> c_pos;

    while (window.isOpen()) {
        sf::Event evnt;
        while (window.pollEvent(evnt)) {
            if (evnt.type == evnt.Closed) {
                window.close();
            }
            if (evnt.type == sf::Event::MouseButtonReleased)
            {
                mouse_position = sf::Mouse::getPosition(window);
                pos = sf::Vector2f(mouse_position);
            }
        }
        sf::Time elapsed = clock.getElapsedTime();
        sf::Time elapsed2 = clock2.getElapsedTime();
        if (elapsed >= time_d)
        {
            sprite.setPosition(ch.position);

            float temp = ch.orientation * 180 / M_PI;

            sprite.setRotation(temp);

            arrive(ch, tar, goal_speed, 3, radius_satis, radius_dacc, goal_rotation, max_rotation);
            update(ch);
            
            decisionTree(ch, tar, one, elapsed2, clock2, pos);

            /*if (ch.velocity != sf::Vector2f(0, 0))
            {
                sf::CircleShape crum;
                crum.setRadius(1);
                crum.setOutlineColor(sf::Color::Black);
                crum.setFillColor(sf::Color::Black);
                circles.push_back(crum);
                c_pos.push_back(ch.position);
            }*/
            window.clear(sf::Color::White);
            window.draw(sprite);

            /*for (int i = 0; i < circles.size(); i++)
            {
                circles[i].setPosition(c_pos[i]);
                window.draw(circles[i]);
            }*/

            window.display();
            clock.restart();
        }
        
    }
    return 0;
}