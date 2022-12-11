#ifndef ACTOR_H_
#define ACTOR_H_
#include "GraphObject.h"
#include <iterator>
#include <queue>
#include <stack>
#include <vector>

// constants must be here according to spec
const unsigned int MAX_BOULDERS = 9;
const unsigned int MIN_GOLD_NUGGETS = 2;
const unsigned int MAX_OIL_BARRELS = 21;
const unsigned int MIN_TICKS_CONSUMABLE = 100;
const unsigned int MIN_TICKS_SPAWN_PROTESTER = 25;
const double MAX_PROTESTER = 15.0;

// forward declaration to use studentWorld pointer
class StudentWorld;

class Actor : public GraphObject {
public:
  Actor(bool visible, int imageID, int startX, int startY, Direction dir,
        unsigned int depth, double size = 1.0);

  virtual void doSomething() = 0;
  bool getAlive();
  void setAlive(bool alive);
  virtual ~Actor();

private:
  bool is_alive = true;
};

class Earth : public Actor {
public:
  Earth(int startX, int startY);

  // required because parent is pure virtual
  virtual void doSomething() { return; };
  bool getDiscovered();
  void setDiscovered(bool discovered);

private:
  bool m_discovered = false; // added for the queue??
};

class Boulder : public Actor {
public:
  enum State { stable, waiting, falling, dead };

  Boulder(int startX, int startY, StudentWorld &world);
  virtual void doSomething();
  State getState() { return this->m_state; }
  void setState(State state);
  StudentWorld *getWorld();

private:
  StudentWorld *m_world;
  State m_state = stable;
  int waiting_ticks_elapsed = 0;
};

class Entity : public Actor {
public:
  Entity(int imageID, int startX, int startY, Direction dir,
         StudentWorld &game);

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
  int getWaterUnits();
  void incWater5();
  void decWater();

  void incSonar();
  void decSonar();
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
  struct coord { // for queue
    int x;
    int y;

    coord() {}

    coord(int x, int y) : x(x), y(y) {}
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

  std::queue<coord> *getLocations();
  void setLocations(std::queue<coord> &location);
  std::queue<coord> *getPathOut();
  void setPathOut(std::queue<coord> &path);
  void setLeave();

  std::vector<coord> *getVisited();
  void setVisited(std::vector<coord> &visited);

  // bool compareCoord (const coord &member, const coord &visited);
  bool checkVisited(int x, int y);

  double getUnitsFromTM();
  bool getFacingTM();
  bool lineOfSightTM();
  bool turnsTowardsTM();
  bool chooseDirection(Direction dir);
  enum Direction checkTrapped(Direction dir);

  bool checkTMLR(int x, int y);
  bool checkTMUD(int x, int y, Direction dir);

  void protesterYells();
  bool checkPath(int start, int end, int same, bool changeX);

  virtual void doSomething() = 0;
  void takeDamage(int amount);

  virtual ~Protester();

  std::stack<coord> &getStackPath() { return this->stack_path; }

  void convPathQueue() {
    for (; this->getStackPath().empty() == false; this->getStackPath().pop()) {
      this->getPathOut()->push(this->getStackPath().top());
    }

    for (; this->getPathOut()->empty() == false; this->getPathOut()->pop()) {
      this->getStackPath().push(this->getPathOut()->front());
    }

    for (; this->getStackPath().empty() == false; this->getStackPath().pop()) {
      this->getPathOut()->push(this->getStackPath().top());
    }
  }

private:
  bool m_leaveOilField = false;
  int m_ticksToWait =
      5; // max(0, 3 - current_level_number / 4);          // added (temp)
  bool m_restState = false;
  int m_restTickCount = m_ticksToWait;
  int m_numMoveCurrDir;
  Tunnelman *m_TM;
  int m_lastShouted = 16; // make bools for status & initialize to 0 & true
  bool m_shoutStatus = true;
  int m_lastPerpendicular = 201; // 0
  bool m_perpendicularState = true;

  // have a function that is called to push all the available spot onto the
  // queue
  std::queue<coord> m_locations;
  std::queue<coord> m_pathOut;
  std::vector<coord> m_visited;

  std::stack<coord> stack_path;
};

class RegularProtester : public Protester {
public:
  RegularProtester(StudentWorld &game, Tunnelman &TM);

  virtual void doSomething();
  virtual ~RegularProtester();

private:
};

class Consumable : public Actor {
public:
  Consumable(StudentWorld &world, bool visible, int imageID, int startX,
             int startY, Direction dir, unsigned int depth = 2);

  virtual void doSomething() = 0;
  StudentWorld *getWorld();
  int getTicks();
  void incTicks();

  virtual ~Consumable();

private:
  StudentWorld *m_world;
  int ticks_existed = 0;
};

class OilBarrel : public Consumable {
public:
  OilBarrel(int x, int y, StudentWorld &world);
  virtual void doSomething();
  virtual ~OilBarrel();
};

class Sonar : public Consumable {
public:
  Sonar(StudentWorld &world);

  virtual void doSomething();
  virtual ~Sonar();
};

// class Sonar : public Actor {
// public:
//   Sonar(StudentWorld &world);

//   virtual void doSomething();
//   virtual ~Sonar();

//   StudentWorld *getWorld();

// private:
//   StudentWorld *m_world;
//   int ticks_existed = 0;
// };

class WaterPool : public Actor {
public:
  WaterPool(int x, int y, StudentWorld &world);
  virtual void doSomething();
  virtual ~WaterPool();
  StudentWorld *getWorld();

private:
  StudentWorld *m_world;
  int ticks_existed = 0;
};

class Squirt : public Actor {
public:
  Squirt(int x, int y, Direction dir, StudentWorld &world);
  ~Squirt();

  void doSomething();

  StudentWorld *getWorld();
  int getTicks();
  void incTicks();

private:
  int ticks_alive = 0;
  StudentWorld *m_world;
};

// class GoldNugget: public Consumable{};

bool inRange(int x1, int y1, int x2, int y2, float max_dist = 6.0);

#endif // ACTOR_H_
