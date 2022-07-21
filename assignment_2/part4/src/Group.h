#include <iostream>
#include <vector>
#include "Entity.h"

#ifndef GROUP_H_
#define GROUP_H_

class Group
{
    public:
    Group() {};
    int getSize();
    Entity refEntity1(int i);
    Entity &refEntity(int i);
    void addEntity(const Entity& ent);
    void grouping();
private:
    vector<Entity> group;  
};

#endif