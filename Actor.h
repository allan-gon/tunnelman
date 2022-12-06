#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"
#include <queue>

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
  Earth(int startX, int startY);
    
  // required because parent is pure virtual
  virtual void doSomething() { return; };
    
  // bool getDiscovered();
  // void setDiscovered(bool discovered);
    
private:
  bool m_discovered = false;              // added for the queue??
};

// class Boulder : public Actor {
// public:
//   Boulder(bool visible, int startX, int startY, int imageID = TID_BOULDER,
//           Direction dir = down, unsigned int depth = 1);
//   virtual void doSomething();

// private:
//   bool is_stable = true;
// };

// start of refactor

class Entity : public Actor {
public:
  Entity(int imageID, int startX, int startY , Direction dir, StudentWorld &game);

  void setWorld(StudentWorld &game);
  StudentWorld *getWorld();
    
  virtual void setHitPoints(int hitPoints);
  int getHitPoints();
  virtual void doSomething() = 0;

  virtual ~Entity();

private:
  int m_hitPoints;
  StudentWorld *m_game;
};

class Tunnelman : public Entity {
public:
  Tunnelman(StudentWorld &game);

  // setters & getters
  void setWaterUnits(int waterUnits);
  int getWaterUnits();
  void setSonarCharge(int sonarCharge);
  int getSonarCharge();
  void setGold(int gold);
  int getGold();

  virtual void doSomething();
  virtual ~Tunnelman();

private:
  int m_waterUnits = 5;
  int m_sonarCharge = 1;
  int m_gold = 0;
};

class Protester : public Entity {
public:
    
    struct coord {        // for queue
        int x;
        int y;
    };
    
    Protester(int imageID, StudentWorld &game, Tunnelman &TM);
    
    void setLeaveStatus(bool leaveOilField);
    bool getLeaveStatus();
    
    void setWaitTicks(int numWait);
    int getWaitTicks();
    
    void setRestState(bool restState);
    bool getRestState();
    
    void setRestTickCount(int numRestTick);
    int getRestTickCount();
    void decRestTickCount();
    
    void setMovesCurrDir(int numCurrDir);
    int getMovesCurrDir();
    void initMovesCurrDir();
    
    void setTM(Tunnelman &TM);
    Tunnelman *getTM();
    
    void setLastShouted(int numLastShouted);
    int getLastShouted();
    
    void setLastPerpendicular(int numLastPerpendicular);
    int getLastPerpendicular();
    
    double getUnitsFromTM();
    bool getFacingTM();
    bool lineOfSightTM();
    bool turnsTowardsTM();
    bool chooseDirection(Direction dir);
    enum Direction checkTrapped(Direction dir);
    
    void protesterYells();
    bool checkPath(int start, int end, int same, bool changeX);
    
    virtual void doSomething() = 0;
    virtual ~Protester();
    
private:
    bool m_leaveOilField = false;
    int m_ticksToWait = 5;  // max(0, 3 - current_level_number / 4);          // added (temp)
    bool m_restState = false;
    int m_restTickCount = m_ticksToWait;
    int m_numMoveCurrDir;
    Tunnelman *m_TM;
    int m_lastShouted = 16;                 // make bools for status & initialize to 0 & true
    bool m_shoutStatus = true;
    int m_lastPerpendicular = 201;          // 0
    bool m_perpendicularState = true;
    
    // have a function that is called to push all the available spot onto the queue
    std::queue<coord> m_locations;
    std::queue<coord> m_pathOut;
};

class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld &game, Tunnelman &TM);
    
    virtual void doSomething();
    virtual ~RegularProtester();
private:
};

#endif // ACTOR_H_
