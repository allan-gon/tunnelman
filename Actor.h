#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include "StudentWorld.h"

// Students:  Add code to this file, Actor.cpp, StudentWorld.h, and StudentWorld.cpp
// comment

class Entity : public GraphObject {
public:
    Entity(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, int hit_points): GraphObject(imageID, startX, startY, dir, size, depth), m_hitPoints(hit_points)
    {
        setVisible(true);
    }

    ~Entity();
private:
    int m_hitPoints = 0;
};

class Tunnelman : public Entity {
public:
    Tunnelman(int imageID, int startX, int startY, Direction dir, double size, unsigned int depth, int hit_points, int water_units, int sonar_charge, int gold): Entity(TID_PLAYER, 30, 60, right, 1.0, 0, 10) //, m_waterUnits(water_units), m_sonarCharge(sonar_charge), m_gold(gold)
        {
            // setVisible(true);
        }
    
    ~Tunnelman();
private:
    int m_waterUnits = 5;
    int m_sonarCharge = 1;
    int m_gold = 0;
};

#endif // ACTOR_H_
