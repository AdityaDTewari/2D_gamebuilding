#include "Vectors.h"
#include <vector>
#include <stdlib.h>
#include <iostream>

#ifndef ENTITY_H_
#define ENTITY_H_

class Entity
{
    public:
    bool follower;
    Vectors position;
    Vectors velocity;
    Vectors acc;
    float max_speed;
    float max_f;
    Entity()    {}
    Entity(float x_pos, float y_pos);
    Entity(float x_pos, float y_pos, bool follow_check);
    void applyF(const Vectors& f);
    
    Vectors Separation(const vector<Entity>& Boids);
    Vectors Alignment(const vector<Entity>& Boids);
    Vectors Cohesion(const vector<Entity>& Boids);
    
    Vectors seek(const Vectors& v);
    void start(const vector<Entity>& v);
    void update();
    void group(const vector<Entity>& v);
    void checkBorders();
    float getAngle(const Vectors& v);
};

#endif