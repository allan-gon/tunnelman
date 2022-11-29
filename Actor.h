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
  Earth(int startX, int startY);
  // required because parent is pure virtual
  virtual void doSomething() { return; };
// private:
    // bool m_marked = false;              // added for the queue??
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
    Protester(int imageID, StudentWorld &game, Tunnelman &TM);
    
    void setLeaveStatus(bool leaveOilField);
    bool getLeaveStatus();
    
    void setWaitTicks(int numWait);
    int getWaitTicks();
    void setRestState(bool restState);
    bool getRestState();
    void setRestTickCount(int numRestTick);
    int getRestTickCount();
    void decRestTickCount();                        // decrements the resting tick count
    void setMovesCurrDir(int numCurrDir);
    int getMovesCurrDir();
    void setTM(Tunnelman &TM);
    Tunnelman *getTM();
    void setLastShouted(int numLastShouted);
    int getLastShouted();
    
    double getUnitsFromTM();                           // distance between Protester & TM
    bool getFacingTM();
    
    virtual void doSomething() = 0;
    virtual ~Protester();
private:
    bool m_leaveOilField = false;
    int m_ticksToWait = 5;  // max(0, 3 - current_level_number / 4);          // added (temp)
    bool m_restState = false;
    int m_restTickCount = m_ticksToWait;
    int m_numMoveCurrDir;
    Tunnelman *m_TM;                    // added to interact w/ TM
    int m_lastShouted = 16;             // starts out being able to shout?
};

class RegularProtester : public Protester {
public:
    RegularProtester(StudentWorld &game, Tunnelman &TM);
    
    virtual void doSomething();
    virtual ~RegularProtester();
private:
};

#endif // ACTOR_H_
