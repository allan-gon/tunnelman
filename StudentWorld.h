#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include <string>
#include <vector>

class StudentWorld : public GameWorld {
public:
  StudentWorld(std::string assetDir) : GameWorld(assetDir) {}

  void populateField();
  void placeBoulders();
  void addProtestor();

  virtual int init();

  virtual int move();

  virtual void cleanUp();

  bool positionClearLR(int x, int y);
  bool positionClearUD(int x, int y);

  int inTMx(int x);
  int inTMy(int y);

  void setEarthDiscovered(int x, int y);
  bool getEarthDiscovered(int x, int y);

  // helper functions for tunnelman
  bool dirtExistsVisible(int x, int y);
  void digDirtLR(int x, int y, bool left);
  void digDirtUD(int x, int y, bool down);

  std::vector<Actor *> &getActors();

  // helper functions for boulder
  bool dirtBelow(int x, int y);
  void clear4by4(int x, int y);
  void boulderAnnoyActors(int x, int y);
  bool boulderObstructs(Actor *object);
  void generateActorCoords(int &x, int &y, int y_left);

  // helper functions for oil barrel
  void decBarrels();
  void placeBarrels();
  Tunnelman *getPlayer();

  // helper functions for sonar
  void calcLifetimeTicks();
  void calcG();
  void placeSonar();
  int getTicks();
  void trySpawnSonarWater();

  // helper functions for waterpool
  void placeWater();
  bool isClear4x4(int x, int y);

  // helper for squirt
  bool squirtAnnoyActors(int x, int y);
  bool dirtObstructs(Actor *object);

  // helper for protester
  bool inBoulderArea(int x, int y);
  bool checkMarked(int x, int y);
  bool findPath(int x, int y, Protester *p);
  std::vector<Protester::coord> *getMarked();
  void calcT();
  void calcP();
  void trySpawnProtestor();
  void decProtesterCount();

  ~StudentWorld();

private:
  int num_protestors = 0;
  int num_barrels_left;
  int ticks_since_p_spawn = 0;
  // intialize multi-dim array of Earth ptrs to nullptr
  Earth *field[VIEW_WIDTH][VIEW_HEIGHT] = {};
  // container for all actors other than tunnel man and earth
  std::vector<Actor *> actors;
  Tunnelman *player = nullptr;
  std::vector<Protester::coord> m_marked;
  int consumable_ticks;
  int G, T, P;
};

#endif // STUDENTWORLD_H_
