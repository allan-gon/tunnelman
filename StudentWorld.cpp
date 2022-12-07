#include "StudentWorld.h"
#include "Actor.h"
#include <algorithm> // this guy's needed because find cries without
#include <fstream>
#include <random>
#include <string>
#include <vector>
using namespace std;

// int num_nuggs = max((5 - this->getLevel()) / 2, MIN_GOLD_NUGGETS);

GameWorld *createStudentWorld(string assetDir) {
  return new StudentWorld(assetDir);
}

bool inRange(int x1, int y1, int x2, int y2, float max_dist) {
  float dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
  return dist <= max_dist;
}

bool intersectShaft(int x) { return ((x > 26) && (x < 34)); }

Tunnelman *StudentWorld::getPlayer() { return this->player; }

void StudentWorld::clear4by4(int x, int y) {
  // assumes x, y are valid (ae. not nullptr and wont raise index out of bounds)
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (this->field[x + i][y + j] == nullptr) {
      } else {
        this->field[x + i][y + j]->setVisible(false);
      }
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

void StudentWorld::generateActorCoords(int &x, int &y, int y_left = 0) {
  // TODO: ask about range
  std::random_device rd;  // obtain random number from harware
  std::mt19937 gen(rd()); // seed the generator
  std::uniform_int_distribution<> x_dist(0, 60); // define the range (inclusive)
  std::uniform_int_distribution<> y_dist(y_left, 56);
  bool generated = false;
  bool broke = false;
  int temp_x, temp_y;
  while (!generated) {
    temp_x = x_dist(gen);
    temp_y = y_dist(gen);
    if (intersectShaft(temp_x)) {
      continue;
    } else {
      for (auto actor : this->actors) {
        if (inRange(temp_x, temp_y, actor->getX(), actor->getY())) {
          broke = true;
          break;
        }
      }
    }

    if (broke) {
      broke = false;
      continue;
    } else {
      x = temp_x;
      y = temp_y;
      generated = true;
    }
  }
}

void StudentWorld::placeBoulders() {
  // calc num boulders for this level
  int num_boulders = min((this->getLevel() / 2) + 2, MAX_BOULDERS);
  int x, y;

  for (int i = 0; i < num_boulders; i++) {
    this->generateActorCoords(x, y, 20);
    this->actors.push_back(std::move(new Boulder(x, y, *this)));
    this->clear4by4(x, y);
  }
}

void StudentWorld::placeBarrels() {
  int num_oil = min(2 + this->getLevel(), MAX_OIL_BARRELS);
  int x, y;

  for (int i = 0; i < num_oil; i++) {
    this->generateActorCoords(x, y);
    std::cout << x << ',' << y << std::endl;
    this->actors.push_back(std::move(new OilBarrel(x, y, *this)));
  }
}

void StudentWorld::addProtestors() {
  // TODO: figure out how many protestors to add
  // for now just adds one protester
  this->actors.push_back(std::move(new RegularProtester(*this, *player)));
}

int StudentWorld::init() {
  this->player = std::move(new Tunnelman(*this));

  this->populateField();
  this->placeBoulders();
  this->placeBarrels();
  this->addProtestors();

  // TODO: spawn all other NEs
  return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move() {
  // TODO: update text
  player->doSomething();

  // destruct actors who are dead on this tick. Have all others
  // doSomething
  for (auto it = this->actors.begin(); it != this->actors.end();) {
    if (!(*it)->getAlive()) {
      delete *it;
      it = this->actors.erase(it);
    } else {
      (*it)->doSomething();
      it++;
    }
  }

  if (this->player->getAlive()) {  // alive
    if (!this->num_barrels_left) { // finished level
      this->playSound(SOUND_FINISHED_LEVEL);
      return GWSTATUS_FINISHED_LEVEL;
    }
    return GWSTATUS_CONTINUE_GAME;
  } else { // dead
    this->decLives();
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
  for (int i = 0; i < this->actors.size(); i++) {
    delete this->actors[i]; // de-alloc
  }
  this->actors.clear(); // empty
}

// dirtExistsVisible
bool StudentWorld::dirtExistsVisible(int x, int y) {
  return (this->field[x][y] != nullptr) && this->field[x][y]->isVisible();
}

void StudentWorld::digDirtLR(int x, int y) {
  bool dug = false;
  for (int i = 0; i < 4; i++) {
    if (this->dirtExistsVisible(x, y + i)) { // ensure there's dirt there
      field[x][y + i]->setVisible(false);
      dug = true;
    }
    if (dug) {
      playSound(SOUND_DIG);
    }
  }
}

void StudentWorld::digDirtUD(int x, int y) {
  bool dug = false;
  for (int i = 0; i < 4; i++) {
    if (this->dirtExistsVisible(x + i, y)) { // ensure dirt is there
      field[x + i][y]->setVisible(false);
      dug = true;
    }
  }
  if (dug) {
    playSound(SOUND_DIG);
  }
}

bool StudentWorld::positionClearLR(int x, int y) {
  // do for loop to check the entire position to prevent walking into dirt
  for (int i = 0; i < 4; i++) {
    if (x < 0 || x > 63 || y > 60 || y < 0 ||
        (field[x][y + i] != nullptr &&
         field[x][y + i]->isVisible())) { // isVisible?
      return false;
    }
  }
  return true;
}

bool StudentWorld::positionClearUD(int x, int y) {
  for (int i = 0; i < 4; i++) {
    if (y > 60 || y < 0 || x > 60 || x < 0 ||
        (field[x + i][y] != nullptr && field[x + i][y]->isVisible())) {
      return false;
    }
  }
  return true;
}

int StudentWorld::inTMx(int x) {
  return (this->player->getX() < x && this->player->getX() + 4 > x);
}

int StudentWorld::inTMy(int y) {
  return (this->player->getY() < y && this->player->getY() + 4 > y);
}

void StudentWorld::setEarthDiscovered(int x, int y) {
  field[x][y]->setDiscovered(true);
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

bool StudentWorld::boulderObstructs(Actor *object) {
  // check is boulder is within +/- 3 of specific actor
  // accessed dir direction - 1
  int dir_modifier[4] = {4, -4, -4, 4};
  bool is_vertical = false;
  if ((object->getDirection() == 1) || (object->getDirection() == 2)) {
    is_vertical = true;
  }

  for (auto actor : this->actors) {
    if (actor->getID() == TID_BOULDER) {
      if (is_vertical) {
        for (int i = -3; i < 4; i++) {
          if ((object->getX() + i == actor->getX()) &&
              (object->getY() + dir_modifier[object->getDirection() - 1] ==
               actor->getY())) {
            return true;
          }
        }
      } else {
        for (int i = -3; i < 4; i++) {
          if ((object->getX() + dir_modifier[object->getDirection() - 1] ==
               actor->getX()) &&
              (object->getY() + i == actor->getY()))
            return true;
        }
      }
    }
  }
  return false;
}

void StudentWorld::boulderAnnoyActors(int x, int y) {
  // really hate there being duplicate coed here.
  // consider adding tunnelman to actor vector
  if (inRange(this->player->getX(), this->player->getY(), x, y, 3)) {
    this->player->setAlive(false);
    return; // Nothing else must be checked since player died
  }

  for (auto actor : this->actors) {
    // only other actors that can be annoyed?
    if ((actor->getID() == TID_PROTESTER) ||
        (actor->getID() == TID_HARD_CORE_PROTESTER)) {
      if (inRange(actor->getX(), actor->getY(), x, y, 3)) {
        // actor->setLeaveStatus(true);
      }
    }
  }
}

StudentWorld::~StudentWorld() {}
