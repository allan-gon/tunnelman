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

bool inRange(int x1, int y1, int x2, int y2, float max_dist = 6.0) {
  float dist = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
  return dist > max_dist;
}

// bool validSpawn(int x1, int y1, int x2, int y2) {
//   // returns whether or not the distance between point 1
//   // and point 2 is > 6. Helper for StudentWorld::init
//   float dist = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
//   return dist > 6;
// }

void StudentWorld::clear4by4(int x, int y) {
  // assumes x, y are valid (ae. not nullptr and wont raise index out of bounds)
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      this->field[x + i][y + j]->setVisible(false);
    }
  }
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

void StudentWorld::placeBoulders() {
  // for now places a single boulder in a fixed location
  this->actors.push_back(std::move(new Boulder(10, 30, *this)));
  this->clear4by4(10, 30);
}

int StudentWorld::init() {
  // initializes tunnelman & provides the studentWorld address
  this->player = std::move(new Tunnelman(*this));

  // place earth in field
  this->populateField();
  // place boulders
  this->placeBoulders();
  // TODO: spawn all other NEs
  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
  // TODO: update text
  player->doSomething();

  for (auto actor : this->actors) {
    actor->doSomething();
  }

  // TODO: check if when 3 lives are exhausted game gracfully kys
  if (this->player->getAlive()) {  // alive
    if (!this->num_barrels_left) { // finished level
      this->playSound(SOUND_FINISHED_LEVEL);
      return GWSTATUS_FINISHED_LEVEL;
    }
    return GWSTATUS_CONTINUE_GAME;
  } else { // dead
    return GWSTATUS_PLAYER_DIED;
  }
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

// dirtExistsVisible
bool StudentWorld::dirtExistsVisible(int x, int y) {
  return (this->field[x][y] != nullptr) && this->field[x][y]->isVisible();
}

void StudentWorld::digDirtLR(int x, int y) {
  for (int i = 0; i < 4; i++) {
    if (this->dirtExistsVisible(x, y + i)) { // ensure there's dirt there
      field[x][y + i]->setVisible(false);
    }
  }
}

void StudentWorld::digDirtUD(int x, int y) {
  for (int i = 0; i < 4; i++) {
    if (this->dirtExistsVisible(x + i, y)) { // ensure dirt is there
      field[x + i][y]->setVisible(false);
    }
  }
}

bool StudentWorld::dirtBelow(int x, int y) {
  // if atleast one dirt block exists directly under 4 blocks from a location
  // return true
  for (int i = 0; i < 4; i++) {
    if (this->dirtExistsVisible(x + i, y - 1)) {
      return true;
    }
  }
  return false;
}

bool StudentWorld::boulderExistsUnder(int x, int y) {
  for (auto actor : this->actors) {
    if (actor->getID() == TID_BOULDER) {
      if ((actor->getX() == x) && (actor->getY() == (y - 4))) {
        return true;
      }
    }
  }
  return false;
}

void StudentWorld::boulderAnnoyActors(int x, int y) {
  if (inRange(this->player->getX(), this->player->getY(), x, y, 3)) {
    this->player->setAlive(false);
    return; // Nothing else must be checked since player died
  }
}

StudentWorld::~StudentWorld() {}
