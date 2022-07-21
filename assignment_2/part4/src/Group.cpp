#include "Entity.h"
#include "Group.h"

int Group::getSize()
{
    return group.size();
}

Entity Group::refEntity1(int i)
{
    return group[i];
}

Entity &Group::refEntity(int i)
{
    return group[i];
}

void Group::addEntity(const Entity& ent)
{
    group.push_back(std::move(ent));
}

void Group::grouping() 
{
    for (int i = 0; i < group.size(); i++)
        group[i].start(group);
}