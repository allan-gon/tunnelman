#ifndef STUDENTWORLD_H_
#define STUDENTWORLD_H_

#include "Actor.h"
#include "GameConstants.h"
#include "GameWorld.h"
#include <string>
#include <vector>

// Students:  Add code to this file, StudentWorld.cpp, Actor.h, and Actor.cpp

class StudentWorld : public GameWorld {
public:
  StudentWorld(std::string assetDir) : GameWorld(assetDir) {}

  virtual int init();
  // {
  // 	return GWSTATUS_CONTINUE_GAME;
  // }

  virtual int move();
  // {
  // 	// This code is here merely to allow the game to build, run, and
  // terminate after you hit enter a few times.
  // 	// Notice that the return value GWSTATUS_PLAYER_DIED will cause our
  // framework to end the current level. 	decLives(); 	return
  // GWSTATUS_PLAYER_DIED;
  // }

  virtual void cleanUp();
  // {
  // }
    
    // getting earth field info
    bool dirtExists(int x, int y);
    void digDirtLR(int x, int y);
    void digDirtUD(int x, int y);
    
private:
  Earth *field[VIEW_WIDTH][VIEW_HEIGHT] =
      {}; // intialize multi-dim array of Earth ptrs to nullptr
  std::vector<Boulder *> boulders; // for now vec of
  // boulder should downcast and have all actors in a single vec
    Tunnelman* player = nullptr;
    // likely everything below this line will live in a single
  // vector where we downcast them may use rtti or better, in actor class have a
  // protected variable vector<Protestor*> protestors; vector<Gold*> gold;
  // vector<Oil*> oil;
  // vector<Water*> water;
  // vector<Sonar*> sonar;
};

#endif // STUDENTWORLD_H_
