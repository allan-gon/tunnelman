#include "StudentWorld.h"
#include "Actor.h"
#include <algorithm> // this guy's needed because find cries without
#include <fstream>
#include <random>
#include <string>
#include <vector>
using namespace std;

// In-argueably my condition doesn't catch this

// int num_boulders = min((this->getLevel() / 2) + 2, MAX_BOULDERS);
// int num_nuggs = max((5 - this->getLevel()) / 2, MIN_GOLD_NUGGETS);
// int num_oil = min(2 + this->getLevel(), MAX_OIL_BARRELS);

GameWorld *createStudentWorld(string assetDir) {
  return new StudentWorld(assetDir);
}

bool inRange(int x1, int y1, int x2, int y2, float max_dist = 6.0) {
  float dist = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
  return dist <= max_dist;
}

bool intersectShaft(int x) { return ((x > 26) && (x < 34)); }

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

void StudentWorld::generateBoulderCoords(int &x, int &y) {
  // TODO: ask about range
  std::random_device rd;  // obtain random number from harware
  std::mt19937 gen(rd()); // seed the generator
  std::uniform_int_distribution<> x_dist(0, 60); // define the range (inclusive)
  std::uniform_int_distribution<> y_dist(20, 56);
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
    this->generateBoulderCoords(x, y);
    this->actors.push_back(std::move(new Boulder(x, y, *this)));
    this->clear4by4(x, y);
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
  std::cout << "----------------------\n";
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

void StudentWorld::digDirtLR(int x, int y, bool left) {
  bool dug = false;
  if (left) {
    if (this->inBoulderArea(x, y)) {
      return;
    }
  }
  if (!left) {
    if (this->inBoulderArea(x - 3, y)) {
      return;
    }
  }
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

void StudentWorld::digDirtUD(int x, int y, bool down) {
  bool dug = false;
  if (down) {
    if (this->inBoulderArea(x, y)) {
      return;
    }
  }
  if (!down) {
    if (this->inBoulderArea(x, y - 3)) {
      return;
    }
  }
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
  // passed in: x + 4, y

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
    if ((y > 63) || (y < 0) || (x > 63) || (x < 0) ||
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

bool StudentWorld::getEarthDiscovered(int x, int y) {
  return field[x][y]->getDiscovered();
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
      if (!dynamic_cast<Protester *>(actor)->getLeaveStatus()) {
        if (inRange(actor->getX(), actor->getY(), x, y, 3)) {
          this->playSound(SOUND_SONAR);
          std::cout << "Pos hit: " << actor->getX() << ',' << actor->getY()
                    << std::endl;
          dynamic_cast<Protester *>(actor)->setLeaveStatus(true);
          // dynamic_cast<Protester*>(actor)->setLeave();
          this->getMarked()->clear();
          this->findPath(actor->getX(), actor->getY(),
                         dynamic_cast<Protester *>(actor));
        }
      }
    }
  }
}

bool StudentWorld::inBoulderArea(int x, int y) {
  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      for (auto actor : this->actors) {
        if (actor->getID() == TID_BOULDER) {
          if ((x + i >= actor->getX() && x + i < actor->getX() + 4) &&
              (y + j >= actor->getY() && y + j < actor->getY() + 4)) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool StudentWorld::checkMarked(int x, int y) {
  std::vector<Protester::coord>::iterator it;
  for (it = m_marked.begin(); it < m_marked.end(); it++) {
    if ((it->x == x) && (it->y == y)) {
      return true; // in m_visited already
    }
  }
  return false;
}

std::vector<Protester::coord> *StudentWorld::getMarked() { return &m_marked; }

bool StudentWorld::findPath(int x, int y, Protester *p) {
  Protester::coord curr(x, y);
  this->getMarked()->push_back(curr);
  p->getPathOut()->push(curr);

  if (x == 60 && y == 60) { //  if at end
    p->getPathOut()->pop(); // removed the 1st pos as it's garbage

    // ofstream file("test.txt", std::ios::app);
    // queue<Protester::coord> other;

    // while (!p->getPathOut()->empty()) {
    //   p->getPathOut()->pop();

    //   curr = p->getPathOut()->front();

    //   file << curr.x << ',' << curr.y << "\n";

    //   other.push(curr);
    // }

    // while (!other.empty()) {
    //   curr = other.front();
    //   other.pop();

    //   p->getPathOut()->push(curr);
    // }

    return true;
  } else { // not found way out
           // mark position

    if (y <= 60) {
      // std::cout << "Up: " << boolalpha << this->positionClearUD(x, y + 4) <<
      // ' '
      //           << !this->checkMarked(x, y + 1) << std::endl;
      if (this->positionClearUD(x, y + 4) && !this->checkMarked(x, y + 1) &&
          findPath(x, y + 1, p)) {
        return true;
      }
    }
    if (x <= 60) {
      // std::cout << "Right: " << boolalpha << this->positionClearLR(x + 4, y)
      //           << ' ' << !this->checkMarked(x + 1, y) << std::endl;
      if (this->positionClearLR(x + 4, y) && !this->checkMarked(x + 1, y) &&
          findPath(x + 1, y, p)) {
        return true;
      }
    }
    if (x >= 0) {
      // std::cout << "Left: " << boolalpha << this->positionClearLR(x - 1, y)
      //           << ' ' << !this->checkMarked(x - 1, y) << std::endl;
      if (this->positionClearLR(x - 1, y) && !this->checkMarked(x - 1, y) &&
          findPath(x - 1, y, p)) {
        return true;
      }
    }
    if (y >= 0) {
      // std::cout << "Down: " << boolalpha << this->positionClearUD(x, y - 1)
      //           << ' ' << !this->checkMarked(x, y - 1) << std::endl;
      if (this->positionClearUD(x, y - 1) && !this->checkMarked(x, y - 1) &&
          findPath(x, y - 1, p)) {
        return true;
      }
    }

    // when left, a point, which should be dead end, is not
    // but other dead ends are?
    // causes spazzing

    // std::cout << "Can't go LRUD @: " << p->getPathOut()->front().x << ","
    //           << p->getPathOut()->front().y << std::endl;
    p->getPathOut()->pop();
    return false;
  }
}

StudentWorld::~StudentWorld() {}
