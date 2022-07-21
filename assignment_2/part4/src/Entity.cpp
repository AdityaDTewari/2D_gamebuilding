#include <iostream>
#include <vector>
#include <string>
#include <math.h>
#include "SFML/Graphics.hpp"
#include "Entity.h"


const int win_width = 930;
const int win_len = 625;

Entity::Entity(float x_pos, float y_pos)
{
    acc = Vectors(0, 0);
    velocity = Vectors(rand()%3 - 2, rand()%3 - 2);
    position = Vectors(x_pos, y_pos);
    max_speed = 3.5;
    max_f = 0.5;
}

Entity::Entity(float x_pos, float y_pos, bool follow_check)
{
    follower = follow_check;
    if (follow_check) {
        max_speed = 7.5;
        max_f = 0.5;
        velocity = Vectors(rand()%3 - 1, rand()%3 - 1);
    } else {
        max_speed = 3.5;
        max_f = 0.5;
        velocity = Vectors(rand()%3 - 2, rand()%3 - 2);
    }
    acc = Vectors(0, 0);
    position = Vectors(x_pos, y_pos);
}

void Entity::applyF(const Vectors& f)
{
    acc.vectorAdd(f);
}

Vectors Entity::Separation(const vector<Entity>& ents)
{
    float sep = 20;
    Vectors steer(0, 0);
    int cnt = 0;
    for (int i = 0; i < ents.size(); i++)
    {
        float d = position.dist(ents[i].position);
        if ((d > 0) && (d < sep))
        {
            Vectors diff(0,0);
            diff = diff.vvSub(position, ents[i].position);
            diff.normalize();
            diff.scalarDivi(d);
            steer.vectorAdd(diff);
            cnt++;
        }
        if ((d > 0) && (d < sep) && follower == true && ents[i].follower == true)
        {
            Vectors temp(0, 0);
            temp = temp.vvSub(position, ents[i].position);
            temp.normalize();
            temp.scalarDivi(d);
            steer.vectorAdd(temp);
            cnt++;
        }
        else if ((d > 0) && (d < sep+70) && ents[i].follower == true)
        {
            Vectors temp(0, 0);
            temp = temp.vvSub(position, ents[i].position);
            temp.scalarMult(900);
            steer.vectorAdd(temp);
            cnt++;
        }
    }
    if (cnt > 0)
        steer.scalarDivi((float)cnt);
    if (steer.magnitude() > 0) {
        steer.normalize();
        steer.scalarMult(max_speed);
        steer.vectorSub(velocity);
        steer.limit(max_f);
    }
    return steer;
}

Vectors Entity::Alignment(const vector<Entity>& ents)
{
    float n_dist = 50;
    Vectors temp(0, 0);
    int cnt = 0;
    for (int i = 0; i < ents.size(); i++) {
        float d = position.dist(ents[i].position);
        if ((d > 0) && (d < n_dist))
        {
            temp.vectorAdd(ents[i].velocity);
            cnt++;
        }
    }
    if (cnt > 0)
    {
        temp.scalarDivi((float)cnt);
        temp.normalize();
        temp.scalarMult(max_speed);
        Vectors steer;
        steer = steer.vvSub(temp, velocity);
        steer.limit(max_f);
        return steer;
    }
    else
    {
        Vectors temp(0, 0);
        return temp;
    }
}

Vectors Entity::Cohesion(const vector<Entity>& ents)
{
    float n_dist = 50;
    Vectors temp(0, 0);
    int cnt = 0;
    for (int i = 0; i < ents.size(); i++)
    {
        float d = position.dist(ents[i].position);
        if ((d > 0) && (d < n_dist)) {
            temp.vectorAdd(ents[i].position);
            cnt++;
        }
    }
    if (cnt > 0) {
        temp.scalarDivi(cnt);
        return seek(temp);
    } else {
        Vectors temp(0,0);
        return temp;
    }
}

Vectors Entity::seek(const Vectors& v)
{
    Vectors d;
    d.vectorSub(v);
    d.normalize();
    d.scalarMult(max_speed);
    acc.vvSub(d, velocity);
    acc.limit(max_f);
    return acc;
}

void Entity::update()
{
    acc.scalarMult(.4);
    velocity.vectorAdd(acc);
    velocity.limit(max_speed);
    position.vectorAdd(velocity);
    acc.scalarMult(0);
}

void Entity::start(const vector <Entity>& v)
{
    group(v);
    update();
    checkBorders();
}

void Entity::group(const vector<Entity>& v)
{
    Vectors s = Separation(v);
    Vectors a = Alignment(v);
    Vectors c = Cohesion(v);
    //s.scalarMult(1.5);
    //a.scalarMult(1.0);
    //c.scalarMult(1.0);
    applyF(s);
    applyF(a);
    applyF(c);
}

void Entity::checkBorders()
{
    if (position.x_comp < 0)    position.x_comp += win_len;
    if (position.y_comp < 0)    position.y_comp += win_width;
    if (position.x_comp > 1000) position.x_comp -= win_len;
    if (position.y_comp > 1000) position.y_comp -= win_width;
}

float Entity::getAngle(const Vectors& v)
{
    float ang = (float)(atan2(v.x_comp, -v.y_comp) * 180 / M_PI);
    return ang;
}