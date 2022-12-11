#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include <fstream>
#include <string>
#include <vector>

class StudentWorld : public GameWorld {
public:
  StudentWorld(std::string assetDir) : GameWorld(assetDir) {}

  void populateField();
  void placeBoulders();
  void addProtestors();

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

  // helper functions for boulder
  bool dirtBelow(int x, int y);
  void clear4by4(int x, int y);
  void boulderAnnoyActors(int x, int y);
  bool boulderObstructs(Actor *object);
  void generateBoulderCoords(int &x, int &y);

  bool inBoulderArea(int x, int y);

  bool checkMarked(int x, int y);
  bool findPath(int x, int y, Protester *p, std::ostream &file);
  std::vector<Protester::coord> *getMarked();

  ~StudentWorld();

private:
  // this value needs to be calcd
  int num_barrels_left = 1;
  // intialize multi-dim array of Earth ptrs to nullptr
  Earth *field[VIEW_WIDTH][VIEW_HEIGHT] = {};
  // container for all actors other than tunnel man and earth
  std::vector<Actor *> actors;
  Tunnelman *player = nullptr;
  std::vector<Protester::coord> m_marked;
};

#endif // STUDENTWORLD_H_
