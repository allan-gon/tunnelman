#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

// forward declaration to use studentWorld pointer
class StudentWorld;

class Actor : public GraphObject {
public:
  Actor(bool visible, int imageID, int startX, int startY, Direction dir,
        unsigned int depth, double size = 1.0);
    
  virtual void doSomething() = 0;
  bool getAlive();
  void setAlive(bool alive);

private:
  bool is_alive = true;
};

class Earth : public Actor {
public:
  Earth(bool visible, int startX, int startY, int imageID = TID_EARTH,
        Direction dir = right, double size = 0.25, unsigned int depth = 3);
  virtual void doSomething() {
    return;
  }; // actually does nothing but needed because earth and boulder share a
     // parent which has this func as pure virtual
};

class Boulder : public Actor {
public:
  Boulder(bool visible, int startX, int startY, int imageID = TID_BOULDER,
          Direction dir = down, unsigned int depth = 1);
  virtual void doSomething();

private:
  bool is_stable = true;
};

// start of refactor

class Entity : public Actor {
public:
    Entity(int imageID, int startX, int startY, Direction dir,
         unsigned int depth = 0, bool visible = true);
    
    void setWorld(StudentWorld& game);              // 1)
    StudentWorld* getWorld();                       // 1)
    
    virtual void setHitPoints(int hitPoints);
    int getHitPoints();
    virtual void doSomething() = 0;
    
    virtual ~Entity();

private:
    int m_hitPoints;
    // moved the StudentWorld pointer to the Entity class to let Protesters also use
    StudentWorld* m_game;                           // 1)
};

class Tunnelman : public Entity {
public:
    // added a parameter to get the studentWorld address
    Tunnelman(StudentWorld& game, int imageID = TID_PLAYER, int startX = 30, int startY = 60,
            Direction dir = right);
    
    // get studentWorld pointer
    // StudentWorld* getWorld();                    1)
    // setters & getters
    void setWaterUnits(int waterUnits);                 //
    int getWaterUnits();                                //
    void setSonarCharge(int sonarCharge);               //
    int getSonarCharge();                               //
    void setGold(int gold);                             //
    int getGold();                                      //
    
    virtual void doSomething();
    virtual ~Tunnelman();

private:
    int m_waterUnits = 5;
    int m_sonarCharge = 1;
    int m_gold = 0;
    // added StudentWorld pointer
    // StudentWorld* m_game;                        1)
};

class Protester : public Entity {
public:
    Protester(StudentWorld& game, int imageID, int startX = 60, int startY = 60, Direction dir = left);
    
    void setLeaveOilField(bool leave);
    bool getLeaveOilField();
    
    virtual void doSomething() = 0;
    virtual ~Protester();
private:
    bool leave_the_oil_field = false;
};

#endif // ACTOR_H_
