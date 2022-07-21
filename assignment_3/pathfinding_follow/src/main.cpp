#include <SFML/Graphics.hpp>
#include <unistd.h>
#include <iostream>
#include <math.h>
#include <vector>
#include <iterator>
#include <utility>
#include <list>
#include <map>
#include <bits/stdc++.h>
#include <algorithm>

using namespace std;

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

// a star search
double getH(map<double, pair<double, double>> coordinates, double curr_node, double target) {
    auto n_coor = coordinates.find(curr_node);
    auto t_coor = coordinates.find(target);
    double x1 = n_coor->second.first;
    double y1 = n_coor->second.second;
    double x2 = t_coor->second.first;
    double y2 = t_coor->second.second;

    double e_dist = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));

    return e_dist;
}

stack<double> a_star(map<double, vector<vector<double>>> small_graph, map<double, pair<double, double>> coordinates, int start, int target) {
    
    priority_queue<vector<double>, vector<vector<double>>, greater<vector<double>>> open_list;
    map<double, double> closed_list;
    
    int number_of_nodes = 28;
    double csfs[number_of_nodes + 1];
    fill_n(&csfs[0], number_of_nodes + 1, INT_MAX);

    int start_node = start;
    int target_node = target;

    csfs[0] = 0;
    csfs[start_node] = 0;

    //heuristics
    double etcs[number_of_nodes + 1];
    fill_n(&etcs[0], number_of_nodes + 1, INT_MAX);

    //start algo
    etcs[start_node] = csfs[start_node] + getH(coordinates, start_node, target_node);
    open_list.push({etcs[start_node], (double)start_node, 0});
    while (open_list.size() > 0) {
        vector<double> top = open_list.top();
        open_list.pop();
        closed_list.insert({top[1], top[2]});
        //cout << "inserted " << top[1] << " " << top[2] << endl;
        double csf_parent = csfs[(int)top[1]];
        double curr_node = top[1];
        //cout << "current node " << curr_node << endl;
        auto children = small_graph.find(top[1]);
        if (top[1] == target_node) {
            //cout << "found" << endl;
            break;
        }
        if (children == small_graph.end()) {
            //cout << "no children, continue" << endl;
            continue;
        }
        vector<vector<double>> children_list = children->second;
        for (vector<double> child : children_list) {
            int curr_child = (int)child[0];
            double child_csf = child[1] + csf_parent;
            double child_etc = getH(coordinates, curr_child, target_node) + child_csf;
            // in open_list
            //cout << "parent " << curr_node << "and child " << curr_child << endl;
            if (closed_list.find(curr_child) != closed_list.end()) {
                //cout << "continued at " << curr_child << endl;
                continue;
            }
            if (csfs[curr_child] != INT_MAX) {
                // compare
                //cout << "here for " << curr_child << " " << child_csf << endl;
                priority_queue<vector<double>, vector<vector<double>>, greater<vector<double>>> second_open_list;
                while (open_list.size() > 0) {
                    vector<double> second_top = open_list.top();
                    if ((int)second_top[1] == curr_child) {
                        if (etcs[curr_child] > child_etc) {
                            csfs[curr_child] = child_csf;
                            etcs[curr_child] = child_etc;
                            open_list.pop();
                            //cout << "POPED " << second_top[1] << " " << second_top[2] << endl;
                            second_open_list.push({child_etc, second_top[1], curr_node});
                            //cout << "Here" << endl;
                        }
                        else {
                            open_list.pop();
                            second_open_list.push(second_top);
                        }
                    }
                    else {
                        //cout << "PUSHED " << second_top[1] << " " << second_top[2] << endl;
                        second_open_list.push(second_top);
                        open_list.pop();
                    }
                }
                swap(open_list, second_open_list);
            }
            // not in open_list
            else {
                //cout << "not there for " << curr_child << " " << child_csf << " " << child_etc << endl;
                open_list.push({child_etc ,(double)curr_child, curr_node});
                csfs[curr_child] = child_csf;
                etcs[curr_child] = child_etc;
            } 
        }
    }

    //contruct path to travel
    stack <double> path;
    double copy_target = target_node;
    while (true) {
        auto target_loc = closed_list.find(copy_target);
        if (target_loc->second != 0) {
            path.push(target_loc->first);
            copy_target = target_loc->second;
        } else {
            path.push(target_loc->first);
            break;
        }
    }

    return path;
}

double find_closest_node(map<double, pair<double, double>> coordinates, double mx, double my) {
    double min_dist = INT_MAX;
    double node = 0;
    for (auto itr = coordinates.begin(); itr != coordinates.end(); itr++) {
        double x1 = itr->second.first;
        double y1 = itr->second.second;
        double e_dist = sqrt(pow((x1 - mx), 2) + pow((y1 - my), 2));
        if (min_dist > e_dist) {
            min_dist = e_dist;
            node = itr->first;
        }
    }
    return node;
}

int main()
{
    //read files
    string line;
    ifstream input_file("newGraph.txt");
    map<double, vector<vector<double>>> small_graph;
    while (getline(input_file, line)) {
        vector<string> words;
        string word = "";
        line = line + " ";
        for (int i = 0; i < line.length(); i++) {
            if (line[i] != ' ') {
                word = word + line[i];
            }
            else {
                words.push_back(word);
                word = "";
            }
        }
        map<double, vector<vector<double>>>::iterator position = small_graph.find(stod(words[0]));
        if (position != small_graph.end()) {
            position->second.push_back({stod(words[1]), stod(words[2])});
        }
        else{
            small_graph.insert({stod(words[0]), {{stod(words[1]), stod(words[2])}}});
        }
    }
    input_file.close();

    //actual coordinates
    map<double, pair<double, double>> coordinates;
    ifstream input_file2("locations.txt");
    while (getline(input_file2, line)) {
        vector<string> words;
        string word = "";
        line = line + " ";
        for (int i = 0; i < line.length(); i++) {
            if (line[i] != ' ') {
                word = word + line[i];
            }
            else {
                words.push_back(word);
                word = "";
            }
        }
        coordinates.insert({stod(words[0]), {stod(words[1]), stod(words[2])}});
    }
    input_file2.close();

    int start_node = 1;
    int target_node = 28;

    //start building window
    sf::RenderWindow window(sf::VideoMode(1500, 625), "my window", sf::Style::Close | sf::Style::Titlebar);
    
    //rooms and obstacles
    sf::ConvexShape Room1;
    Room1.setPointCount(20);
    Room1.setPoint(0, sf::Vector2f(10, 10));
    Room1.setPoint(1, sf::Vector2f(310, 10));
    Room1.setPoint(2, sf::Vector2f(310, 110));
    Room1.setPoint(3, sf::Vector2f(460, 110));
    Room1.setPoint(4, sf::Vector2f(460, 10));
    Room1.setPoint(5, sf::Vector2f(760, 10));
    Room1.setPoint(6, sf::Vector2f(760, 110));
    Room1.setPoint(7, sf::Vector2f(910, 110));
    Room1.setPoint(8, sf::Vector2f(910, 10));
    Room1.setPoint(9, sf::Vector2f(1210, 10));
    Room1.setPoint(10, sf::Vector2f(1210, 310));
    Room1.setPoint(11, sf::Vector2f(910, 310));
    Room1.setPoint(12, sf::Vector2f(910, 210));
    Room1.setPoint(13, sf::Vector2f(760, 210));
    Room1.setPoint(14, sf::Vector2f(760, 310));
    Room1.setPoint(15, sf::Vector2f(460, 310));
    Room1.setPoint(16, sf::Vector2f(460, 210));
    Room1.setPoint(17, sf::Vector2f(310, 210));
    Room1.setPoint(18, sf::Vector2f(310, 310));
    Room1.setPoint(19, sf::Vector2f(10, 310));
    Room1.setOutlineColor(sf::Color::Blue);
    Room1.setOutlineThickness(2);
    Room1.setPosition(10, 10);

    sf::ConvexShape Room2;
    Room2.setPointCount(4);
    Room2.setPoint(0, sf::Vector2f(110, 110));
    Room2.setPoint(1, sf::Vector2f(210, 110));
    Room2.setPoint(2, sf::Vector2f(210, 210));
    Room2.setPoint(3, sf::Vector2f(110, 210));
    Room2.setFillColor(sf::Color::Blue);
    Room2.setPosition(10, 10);

    sf::ConvexShape Room3;
    Room3.setPointCount(4);
    Room3.setPoint(0, sf::Vector2f(560, 110));
    Room3.setPoint(1, sf::Vector2f(660, 110));
    Room3.setPoint(2, sf::Vector2f(660, 210));
    Room3.setPoint(3, sf::Vector2f(560, 210));
    Room3.setFillColor(sf::Color::Blue);
    Room3.setPosition(10, 10);

    sf::ConvexShape Room4;
    Room4.setPointCount(4);
    Room4.setPoint(0, sf::Vector2f(1010, 110));
    Room4.setPoint(1, sf::Vector2f(1110, 110));
    Room4.setPoint(2, sf::Vector2f(1110, 210));
    Room4.setPoint(3, sf::Vector2f(1010, 210));
    Room4.setFillColor(sf::Color::Blue);
    Room4.setPosition(10, 10);

    //character
    sf::Texture texture;
    texture.loadFromFile("boid-sm.png");
    sf::Sprite sprite(texture);

    Kinematic ch, tar;
    ch.position = sf::Vector2f(1160, 260);
    ch.velocity = sf::Vector2f(0, 0);
    ch.orientation = 0;
    ch.rotation = 0;
    ch.linear = sf::Vector2f(0, 0);
    ch.angular = 0;

    float radius_dacc = 10;
    float radius_satis = 0.5;
    sf::Vector2i mouse_position;
    float goal_speed = 0;
    float goal_rotation = 0;
    float max_rotation = 0.25;

    sf::Clock clock;
    sf::Time time_d = sf::seconds(0.02);
    sf::Clock clock2;
    sf::Time time_d2 = sf::seconds(0.1);

    stack<double> path = a_star(small_graph, coordinates, start_node, target_node);

    double f_node = path.top();
    path.pop();
    auto itr = coordinates.find(f_node);
    double xx = itr->second.first;
    double yy = itr->second.second;
    
    ch.position = sf::Vector2f(xx, yy);
    f_node = path.top();
    path.pop();
    itr = coordinates.find(f_node);
    xx = itr->second.first;
    yy = itr->second.second;
    tar.position = sf::Vector2f(xx, yy);

    sf::Vector2f final_target(0, 0);

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
                //cout << mouse_position.x << " " << mouse_position.y << " ";
                final_target = sf::Vector2f(mouse_position);
                double clos_node = find_closest_node(coordinates, mouse_position.x, mouse_position.y);

                sf::Vector2f sprite_pos = sprite.getPosition();

                double clos_node2 = find_closest_node(coordinates, sprite_pos.x, sprite_pos.y);

                path = a_star(small_graph, coordinates, (int)clos_node2, (int)clos_node);
                path.pop();
                //cout << clos_node << endl;
            }
        }
        sf::Time elapsed = clock.getElapsedTime();
        sf::Time elapsed2 = clock2.getElapsedTime();
        if (elapsed >= time_d)
        {
            sprite.setPosition(ch.position);
            float temp = ch.orientation * 180 / M_PI;
            sprite.setRotation(temp);

            double distance_to_current_target = getLength(tar.position - ch.position);

            if (distance_to_current_target <= 11 && !path.empty()) {
                f_node = path.top();
                path.pop();
                itr = coordinates.find(f_node);
                xx = itr->second.first;
                yy = itr->second.second;
                tar.position = sf::Vector2f(xx, yy);
            }

            else if (path.empty() && (final_target.x != 0 && final_target.y != 0)) {
                tar.position = final_target;
            }

            arrive(ch, tar, goal_speed, 2, radius_satis, radius_dacc, goal_rotation, max_rotation);
            update(ch);

            if (elapsed2 >= time_d2) {
                if (ch.velocity != sf::Vector2f(0, 0)) {
                    sf::CircleShape crum;
                    crum.setRadius(1);
                    crum.setOutlineColor(sf::Color::Black);
                    crum.setFillColor(sf::Color::Black);
                    circles.push_back(crum);
                    c_pos.push_back(ch.position);

                    if (circles.size() > 100) {
                        circles.erase(circles.begin());
                        c_pos.erase(c_pos.begin());
                    }
                }
                clock2.restart();
            }
            


            window.clear(sf::Color::White);
            
            window.draw(Room1);
            window.draw(Room2);
            window.draw(Room3);
            window.draw(Room4);
            window.draw(sprite);
            // draw crumbs
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