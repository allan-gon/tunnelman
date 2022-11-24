#include "StudentWorld.h"
#include "Actor.h"
#include <algorithm> // this guy's needed because find cries without
#include <random>
#include <string>
#include <vector>
using namespace std;

// max number of spawnanble consumables on a single level
const unsigned int MAX_BOULDERS = 9;
const unsigned int MIN_GOLD_NUGGETS = 2;
const unsigned int MAX_OIL_BARRELS = 21;

// int num_boulders = min((this->getLevel() / 2) + 2, MAX_BOULDERS);
// int num_nuggs = max((5 - this->getLevel()) / 2, MIN_GOLD_NUGGETS);
// int num_oil = min(2 + this->getLevel(), MAX_OIL_BARRELS);

GameWorld *createStudentWorld(string assetDir) {
  return new StudentWorld(assetDir);
}

bool validSpawn(int x1, int y1, int x2, int y2) {
  // returns whether or not the distance between point 1
  // and point 2 is > 6. Helper for StudentWorld::init
  float dist = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
  return dist > 6;
}

void StudentWorld::populateField() {
  for (int row = 0; row < VIEW_HEIGHT; row++) {
    for (int col = 0; col < VIEW_WIDTH; col++) {
      if (row < 4) {
        this->field[col][row] = std::move(new Earth(col, row));
      } else if (((col < 30) || (col > 33)) && (row < 60)) {
        this->field[col][row] = std::move(new Earth(col, row));
      }
    }
  }
}

int StudentWorld::init() {
  // initializes tunnelman & provides the studentWorld address
  this->player = std::move(new Tunnelman(*this));
  // place earth in field
  this->populateField();
  // TODO: spawn all other NEs
  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
  player->doSomething();

  for (auto actor : this->actors) {
    actor->doSomething();
  }

  return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp() {
  // frees memory from tunnelman + fix dangling pointer
  delete this->player;
  this->player = nullptr;

  // frees memory from Earth object + fix dangling pointer
  for (int row = 0; row < VIEW_HEIGHT; row++) {
    for (int col = 0; col < VIEW_WIDTH; col++) {
      if (this->field[col][row] != nullptr) {
        delete this->field[col][row];
        this->field[col][row] = nullptr;
      }
    }
  }

  // free's any memory that was allocated to actors
  for (int i = this->actors.size(); i >= 0; i--) {
    delete this->actors[i];
    this->actors.erase(this->actors.begin() + i - 1);
  }
}

// dirtExists
bool StudentWorld::dirtExists(int x, int y) {
  return this->field[x][y] != nullptr;
}

void StudentWorld::digDirtLR(int x, int y) {
  for (int i = 0; i < 4; i++) {
    if (this->dirtExists(x, y + i)) { // ensure there's dirt there
      field[x][y + i]->setVisible(false);
    }
  }
}

void StudentWorld::digDirtUD(int x, int y) {
  for (int i = 0; i < 4; i++) {
    if (this->dirtExists(x + i, y)) { // ensure dirt is there
      field[x + i][y]->setVisible(false);
    }
  }
}

StudentWorld::~StudentWorld() {}
