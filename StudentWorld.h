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

  virtual int init();

  virtual int move();

  virtual void cleanUp();

  // getting earth field info
  bool dirtExistsVisible(int x, int y);
  void digDirtLR(int x, int y);
  void digDirtUD(int x, int y);
  bool dirtVisible(int x, int y);
    bool positionClearLR(int x, int y);
    bool positionClearUD(int x, int y);
    
  ~StudentWorld();

private:
  // intialize multi-dim array of Earth ptrs to nullptr
  Earth *field[VIEW_WIDTH][VIEW_HEIGHT] = {};
  // container for all actors other than tunnel man and earth
  std::vector<Actor *> actors;
  Tunnelman *player = nullptr;
    Protester *p1 = nullptr;                        // testing regular protester
};

#endif // STUDENTWORLD_H_
