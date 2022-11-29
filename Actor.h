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
};

class Boulder : public Actor {
public:
  enum State { stable, waiting, falling, dead };

  Boulder(int startX, int startY, StudentWorld &world);
  virtual void doSomething();
  bool getState();
  void setState(State state);
  StudentWorld *getWorld();

private:
  StudentWorld *m_world;
  State m_state = stable;
  int waiting_ticks_elapsed = 0;
};

class Entity : public Actor {
public:
  Entity(int imageID, int startX, int startY);

  virtual void setHitPoints(int hitPoints);
  int getHitPoints();
  virtual void doSomething() = 0;

  virtual ~Entity();

private:
  int m_hitPoints;
};

class Tunnelman : public Entity {
public:
  Tunnelman(StudentWorld &game);

  // get studentWorld pointer
  StudentWorld *getWorld();
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
  // added StudentWorld pointer
  StudentWorld *m_game;
};

#endif // ACTOR_H_
