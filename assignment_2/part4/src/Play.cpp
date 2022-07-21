#include <iostream>
#include "Group.h"
#include "Entity.h"
#include "Vectors.h"
#include "Play.h"
#include "SFML/Graphics.hpp"

Play::Play()
{
    this->ent_size = 3.0;
    this->window.create(sf::VideoMode(930, 625), "Part 4", sf::Style::Close);
    
    window.setFramerateLimit(60);
}

void Play::start()
{
    for (int i = 0; i < 100; i++)
    {
        Entity ent(930 / 3, 625 / 3);
        sf::CircleShape ents(8, 3);
        ents.setOutlineColor(sf::Color(0,255,0));
        ents.setFillColor(sf::Color::Green);
        ents.setOutlineColor(sf::Color::Black);
        ents.setOutlineThickness(1);
        ents.setRadius(ent_size);
        group.addEntity(ent);
        ent_g.push_back(ents);
    }
    while (window.isOpen())
    {
        inputs();
        display();
    }
}

void Play::inputs()
{
    sf::Event event;
    while (window.pollEvent(event))
    {
        if ((event.type == sf::Event::Closed) || (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape))
        {
            window.close();
        }
    }
}

void Play::display()
{
    window.clear(sf::Color::White);
    for (int i = 0; i < ent_g.size(); i++)
    {
        window.draw(ent_g[i]);
        ent_g[i].setPosition(group.refEntity(i).position.x_comp, group.refEntity(i).position.y_comp);
        float ang = group.refEntity(i).getAngle(group.refEntity(i).velocity);
        ent_g[i].setRotation(ang);
        if (ent_g[i].getPosition().x > 930)
        {
            ent_g[i].setPosition(ent_g[i].getPosition().x - 930, ent_g[i].getPosition().y);
        }
        if (ent_g[i].getPosition().y > 625)
        {
            ent_g[i].setPosition(ent_g[i].getPosition().x, ent_g[i].getPosition().y - 625);
        }
        if (ent_g[i].getPosition().x < 0)
        {
            ent_g[i].setPosition(ent_g[i].getPosition().x + 930, ent_g[i].getPosition().y);
        }
        if (ent_g[i].getPosition().y < 0)
        {
            ent_g[i].setPosition(ent_g[i].getPosition().x, ent_g[i].getPosition().y + 625);
        }
    }

    group.grouping();
    window.display();
}