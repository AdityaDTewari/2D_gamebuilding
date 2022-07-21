#include <iostream>
#include "Group.h"
#include "Entity.h"
#include "Vectors.h"
#include "SFML/Window.hpp"
#include "SFML/Graphics.hpp"

#ifndef PLAY_H
#define PLAY_H

class Play {
private:
    sf::RenderWindow window;

    Group group;
    float ent_size;
    vector<sf::CircleShape> ent_g;

    void display();
    void inputs();

public:
    Play();
    void start();
};

#endif