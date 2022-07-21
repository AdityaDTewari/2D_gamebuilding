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
    bool dance;
    bool m_p;
    bool n_p;
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

bool condition4(Kinematic &obj, Kinematic &obj2)
{
    sf::Vector2f temp = obj2.position - obj.position;
    float length = sqrt((temp.x * temp.x) + (temp.y * temp.y));
    if (length < 10)
    {
        //std::cout << "collision" << std::endl;
        return true;
    }
    else
    {
        //std::cout << "non" << std::endl;
        //std::cout << length << std::endl;
        return false;
    }
}

void reset(Kinematic &obj, Kinematic &obj2, Kinematic &obj3, Kinematic &obj4)
{
    //std::cout << "reset" << std::endl;
    obj.position = sf::Vector2f(0, 625);
    obj.velocity = sf::Vector2f(0, 0);
    obj.orientation = 0;
    obj.rotation = 0;
    obj.linear = sf::Vector2f(0, 0);
    obj.angular = 0;
    obj.m_p = false;
    obj.n_p = true;
    //obj.dance = false;

    obj2.position = sf::Vector2f(0, 0);

    obj3.position = sf::Vector2f(0, 0);
    obj3.velocity = sf::Vector2f(0, 0);
    obj3.orientation = 0;
    obj3.rotation = 0;
    obj3.linear = sf::Vector2f(0, 0);
    obj3.angular = 0;
    obj3.dance = false;
    obj3.m_p = false;
    obj3.n_p = false;

    obj4.position = sf::Vector2f(970, 300);
}

void dance (Kinematic &obj1, Kinematic &obj2)
{
    //std::cout << "dance" << std::endl;
    obj1.dance = true;
}

void action4(Kinematic &obj, Kinematic &obj2, Kinematic &obj3, Kinematic &obj4)
{
    //std::cout << "Action4" << std::endl;
    dance(obj, obj3);
    reset(obj, obj2, obj3, obj4);
}

bool condition5(Kinematic &obj)
{
    if (obj.m_p || obj.n_p)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void action5(Kinematic &obj, Kinematic &obj2, Kinematic &obj3)
{
    if (obj.m_p)
    {
        //std::cout << "here" << std::endl;
        obj.n_p = false;
        obj.m_p = false;
        std::random_device rd3;
        std::mt19937 rng3(rd3());
        std::uniform_int_distribution<int> uni3(-1000,1000);
        float f1 = uni3(rng3);
        float f2 = uni3(rng3);
        //std::cout << f1 << " " << f2 << std::endl;
        obj2.position = sf::Vector2f(f1, f2);
    }
    if (obj.n_p)
    {
        //std::cout << "over here" << std::endl;
        obj.m_p = false;
        obj2.position = obj3.position;
    }
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
            else if (node->val == "C4")
            {
                break;
            }
            else if (node->val == "C5")
            {
                break;
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

void behaviourTree(Kinematic &obj, Kinematic &obj2, struct Node* node, sf::Time &time, sf::Clock &clk, sf::Vector2f &pos, Kinematic &obj3, Kinematic &obj4)
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
            else if (node->val == "C4")
            {
                ans = condition4(obj, obj2);
                if (ans)
                {
                    node = node->left;
                }
                else
                {
                    node = node->right;
                }
            }
            else if (node->val == "C5")
            {
                ans = condition5(obj);
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
                action1(obj);
                break;
            }

            else if(node->val == "A2")
            {
                action2(obj, time, clk);
                break;
            }

            else if (node->val == "A3")
            {
                action3(obj2, pos);
                break;
            }
            else if (node->val == "A4")
            {
                action4(obj, obj2, obj3, obj4);
                break;
            }
            else if (node->val == "A5")
            {
                action5(obj, obj2, obj3);
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
    Node* seven = new Node;
    Node* eight = new Node;
    Node* nine = new Node;
    Node* ten = new Node;
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
    five->right = seven;
    six->val = "A3";
    six->left = e1;
    six->right = e2;
    seven->val = "C4";
    seven->left = eight;
    seven->right = nine;
    eight->val = "A4";
    eight->left = e1;
    eight->right = e2;
    nine->val = "C5";
    nine->left = ten;
    nine->right = e2;
    ten->val = "A5";
    ten->left = e1;
    ten->right = e2;
    e1->left = NULL;
    e1->right = NULL;
    e2->left = NULL;
    e2->right = NULL;

    sf::RenderWindow window(sf::VideoMode(930, 625), "my window", sf::Style::Close | sf::Style::Titlebar);

    int b_count = 0;

    sf::Texture texture;
    sf::Texture texture2;
    texture.loadFromFile("boid-sm.png");
    texture2.loadFromFile("boid_2.png");
    sf::Sprite sprite(texture);

    //added line
    sf::Sprite sprite2(texture2);

    Kinematic ch, tar;

    //added line
    Kinematic monster, tar_mon;

    ch.position = sf::Vector2f(0, 0);
    ch.velocity = sf::Vector2f(0, 0);
    ch.orientation = 0;
    ch.rotation = 0;
    ch.linear = sf::Vector2f(0, 0);
    ch.angular = 0;
    ch.dance = false;
    ch.m_p = false;
    ch.n_p = false;

    //added line
    monster.position = sf::Vector2f(0, 625);
    monster.velocity = sf::Vector2f(0, 0);
    monster.orientation = 0;
    monster.rotation = 0;
    monster.linear = sf::Vector2f(0, 0);
    monster.angular = 0;
    monster.dance = false;
    monster.m_p = false;
    monster.n_p = true;

    float radius_dacc = 20;
    float radius_satis = 0;
    sf::Vector2i mouse_position(0, 0);
    sf::Vector2f pos = sf::Vector2f(0, 0);
    //tar.position = sf::Vector2f(mouse_position);
    tar.position = sf::Vector2f(970, 300);
    float goal_speed = 0;
    float goal_rotation = 0;
    float max_rotation = 0.25;

    //added line
    float radius_dacc_mon = 1;
    float radius_satis_mon = 0;
    sf::Vector2i mouse_position_mon(0, 0);
    sf::Vector2f pos_mon = sf::Vector2f(0, 0);
    //tar.position = sf::Vector2f(mouse_position);
    tar_mon.position = sf::Vector2f(970, 300);
    float goal_speed_mon = 0;
    float goal_rotation_mon = 0;
    float max_rotation_mon = 0.25;


    sf::Clock clock;
    sf::Time time_d = sf::seconds(0.02);
    sf::Clock clock2;
    
    //added line
    sf::Clock new_clock;
    sf::Time time_d2 = sf::seconds(0.5);

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
            if (evnt.type == sf::Event::KeyPressed)
            {
                if (evnt.key.code == sf::Keyboard::M)
                {
                    //std::cout << "the M key was pressed" << std::endl;
                    monster.m_p = true;
                }
                if (evnt.key.code == sf::Keyboard::N)
                {
                    monster.n_p = true;
                }
            }
        }
        sf::Time elapsed = clock.getElapsedTime();
        sf::Time elapsed2 = clock2.getElapsedTime();
        sf::Time elapsed3 = new_clock.getElapsedTime();
        if (elapsed >= time_d)
        {
            sprite.setPosition(ch.position);

            //added line
            sprite2.setPosition(monster.position);

            float temp = ch.orientation * 180 / M_PI;

            //added line
            float temp_mon = monster.orientation * 180 / M_PI;

            sprite.setRotation(temp);

            //added line
            sprite2.setRotation(temp_mon);

            if (monster.n_p)
            {
                tar_mon = ch;
            }
            //tar_mon = ch;

            arrive(ch, tar, goal_speed, 4, radius_satis, radius_dacc, goal_rotation, max_rotation);
            update(ch);
            
            decisionTree(ch, tar, one, elapsed2, clock2, pos);

            //added line
            arrive(monster, tar_mon, goal_speed_mon, 1, radius_satis_mon, radius_dacc_mon, goal_rotation_mon, max_rotation_mon);
            update(monster);
            behaviourTree(monster, tar_mon, one, elapsed2, clock2, pos_mon, ch, tar);

            if (monster.dance == true)
            {
                if (elapsed3 >= time_d2)
                {
                    new_clock.restart();
                    std::random_device rd2;
                    std::mt19937 rng2(rd2());
                    std::uniform_int_distribution<int> uni2(300, 500);
                    float f1 = uni2(rng2);
                    float f2 = uni2(rng2);
                    monster.position = sf::Vector2f(f1, f2);
                    sprite2.setPosition(monster.position);
                    b_count++;
                }
                if (b_count == 5)
                {
                    b_count = 0;
                    monster.dance = false;
                    reset(monster, tar_mon, ch, tar);
                    new_clock.restart();
                }

                window.clear(sf::Color::White);
                window.draw(sprite2);
            }

            /*if (ch.velocity != sf::Vector2f(0, 0))
            {
                sf::CircleShape crum;
                crum.setRadius(1);
                crum.setOutlineColor(sf::Color::Black);
                crum.setFillColor(sf::Color::Black);
                circles.push_back(crum);
                c_pos.push_back(ch.position);
            }*/
            
            else
            {
                window.clear(sf::Color::White);
                window.draw(sprite);
                window.draw(sprite2);
            }
            //window.clear(sf::Color::White);
            //window.draw(sprite);

            //added line
            //window.draw(sprite2);

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