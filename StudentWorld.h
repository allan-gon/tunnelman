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
  bool dirtExists(int x, int y);
  void digDirtLR(int x, int y);
  void digDirtUD(int x, int y);

private:
  Earth *field[VIEW_WIDTH][VIEW_HEIGHT] = {};
  // intialize multi-dim array of Earth ptrs to nullptr
  // std::vector<Boulder *> boulders; // for now vec of
  // boulder should downcast and have all actors in a single vec
  Tunnelman *player = nullptr;
  // likely everything below this line will live in a single
  // vector where we downcast them may use rtti or better, in actor class have a
  // protected variable vector<Protestor*> protestors; vector<Gold*> gold;
  // vector<Oil*> oil;
  // vector<Water*> water;
  // vector<Sonar*> sonar;
};

#endif // STUDENTWORLD_H_
