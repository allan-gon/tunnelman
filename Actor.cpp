#include "Actor.h"
#include "StudentWorld.h"
#include <iostream>
#include <random>

Actor::Actor(bool visible, int imageID, int startX, int startY, Direction dir,
             unsigned int depth, double size)
    : GraphObject(imageID, startX, startY, dir, size, depth) {
  this->setVisible(visible);
}

bool Actor::getAlive() { return this->is_alive; }

void Actor::setAlive(bool alive) { this->is_alive = alive; }

Actor::~Actor(){};

Earth::Earth(int startX, int startY)
    : Actor(true, TID_EARTH, startX, startY, right, 3, 0.25) {}

bool Earth::getDiscovered() { return m_discovered; }

void Earth::setDiscovered(bool discovered) { m_discovered = discovered; }

Boulder::Boulder(int startX, int startY, StudentWorld &world)
    : Actor(true, TID_BOULDER, startX, startY, down, 1), m_world(&world) {}

void Boulder::setState(State state) { this->m_state = state; }

StudentWorld *Boulder::getWorld() { return this->m_world; }

void Boulder::doSomething() {
  if (this->getAlive()) {             // if alive
    if (this->getState() == stable) { // if stable
      if (!(this->getWorld()->dirtBelow(
              this->getX(), this->getY()))) { // if no dirt in 4 squares below
        this->setState(waiting);              // now waiting
      }

    } else if (this->getState() == waiting) { // if waiting
      if (this->waiting_ticks_elapsed > 29) { // if 30 ticks have elapsed
        this->getWorld()->playSound(SOUND_FALLING_ROCK);
        this->setState(falling); // now falling
      } else {                   // if <= 30 ticks
        this->waiting_ticks_elapsed++;
      }
    } else if (this->getState() == falling) { // if falling
      // if not at the bottom and no dirt or earth directly below
      if ((this->getY() > 0) && !this->getWorld()->boulderObstructs(this) &&
          !this->getWorld()->dirtBelow(this->getX(), this->getY())) {
        this->getWorld()->boulderAnnoyActors(this->getX(), this->getY());
        this->moveTo(this->getX(), this->getY() - 1); // move down 1 row
      } else {
        this->setState(dead);
        this->setAlive(false);
      }
    } else if (this->getState() == dead) {
      this->setVisible(false);
    }
  }
}

Entity::Entity(int imageID, int startX, int startY, Direction dir,
               StudentWorld &game)
    : Actor(true, imageID, startX, startY, dir, 0), m_game(&game) {}

void Entity::setHitPoints(int hitPoints) { this->m_hitPoints = hitPoints; }

int Entity::getHitPoints() { return this->m_hitPoints; }

// getter & setter m_game Entity class
void Entity::setWorld(StudentWorld &game) { m_game = &game; }
StudentWorld *Entity::getWorld() { return m_game; }

void Entity::doSomething() { return; }

void Entity::annoy() { return; }

Entity::~Entity() {}

// added the StudentWorld address & initializes the member variable
Tunnelman::Tunnelman(StudentWorld &game)
    : Entity(TID_PLAYER, 30, 60, right, game) {
  this->setHitPoints(10);
}

void Tunnelman::setWaterUnits(int waterUnits) {
  this->m_waterUnits = waterUnits;
}

int Tunnelman::getWaterUnits() { return this->m_waterUnits; }

void Tunnelman::setSonarCharge(int sonarCharge) {
  this->m_sonarCharge = sonarCharge;
}

int Tunnelman::getSonarCharge() { return this->m_sonarCharge; }

void Tunnelman::setGold(int gold) { this->m_gold = gold; }

int Tunnelman::getGold() { return this->m_gold; }

void Tunnelman::doSomething() {
  int key;
  // if player is alive and a key was pressed
  if (this->getAlive() && this->getWorld()->getKey(key)) {
    switch (key) {
    case KEY_PRESS_LEFT:
      if (this->getDirection() == left) { // if already facing in that direction
        if (this->getX() != 0) {          // if not on edge
          this->getWorld()->digDirtLR(this->getX() - 1, this->getY(), true);
          if (!this->getWorld()->boulderObstructs(this)) {
            this->moveTo(this->getX() - 1, this->getY());
          }
        }
      } else {
        this->setDirection(left);
      }
      break;
    case KEY_PRESS_RIGHT:
      if (this->getDirection() == right) {
        if (this->getX() != 60) {
          this->getWorld()->digDirtLR(this->getX() + 4, this->getY(), false);
          if (!this->getWorld()->boulderObstructs(this)) {
            this->moveTo(this->getX() + 1, this->getY());
          }
        }
      } else {
        this->setDirection(right);
      }
      break;
    case KEY_PRESS_DOWN:
      if (this->getDirection() == down) {
        if (this->getY() != 0) {
          this->getWorld()->digDirtUD(this->getX(), this->getY() - 1, true);
          if (!this->getWorld()->boulderObstructs(this)) {
            this->moveTo(this->getX(), this->getY() - 1);
          }
        }
      } else {
        this->setDirection(down);
      }
      break;
    case KEY_PRESS_UP:
      if (this->getDirection() == up) {
        if (this->getY() != 60) {
          if (this->getY() < 60) { // padding since tunnel man takes a 4x4
            this->getWorld()->digDirtUD(this->getX(), this->getY() + 4, false);
          }
          if (!this->getWorld()->boulderObstructs(this)) {
            this->moveTo(this->getX(), this->getY() + 1);
          }
        }
      } else {
        this->setDirection(up);
      }
      break;
      // =================================================================================================
    case KEY_PRESS_ESCAPE:
      this->setAlive(false);
      break;
    case KEY_PRESS_TAB:
      break;
    case KEY_PRESS_SPACE:
      break;
      // =================================================================================================
    default:
      break;
    }
  }
}

void Tunnelman::annoy() {
  // possibly should increase annoyance
  this->setHitPoints(this->getHitPoints() - 100);
}

Tunnelman::~Tunnelman() {}

Protester::Protester(int imageID, StudentWorld &game, Tunnelman &TM)
    : Entity(imageID, 60, 60, left, game), m_TM(&TM) {
  this->initMovesCurrDir();
}
void Protester::annoy() { return; }

void Protester::doSomething() { return; }

void Protester::setLeaveStatus(bool leaveOilField) {
  m_leaveOilField = leaveOilField;
}

bool Protester::getLeaveStatus() { return m_leaveOilField; }

void Protester::setWaitTicks(int numWait) { m_ticksToWait = numWait; }

int Protester::getWaitTicks() { return m_ticksToWait; }

void Protester::setRestState(bool restState) { m_restState = restState; }

bool Protester::getRestState() { return m_restState; }

void Protester::setRestTickCount(int numRestTick) {
  m_restTickCount = numRestTick;
}

int Protester::getRestTickCount() { return m_restTickCount; }

void Protester::decRestTickCount() {
  this->setRestTickCount(this->getRestTickCount() - 1);
  if (this->getRestTickCount() == 0) {
    this->setRestState(false);
  }
}

void Protester::setMovesCurrDir(int numCurrDir) {
  m_numMoveCurrDir = numCurrDir;
}

int Protester::getMovesCurrDir() { return m_numMoveCurrDir; }

void Protester::initMovesCurrDir() {
  int val;
  val = (rand() % 53) + 8;
  m_numMoveCurrDir = val;
}

void Protester::setTM(Tunnelman &TM) { m_TM = &TM; }

Tunnelman *Protester::getTM() { return m_TM; }

void Protester::setLastShouted(int numLastShouted) {
  m_lastShouted = numLastShouted;
}

int Protester::getLastShouted() { return m_lastShouted; }

void Protester::setLastPerpendicular(int numLastPerpendicular) {
  m_lastPerpendicular = numLastPerpendicular;
}

int Protester::getLastPerpendicular() { return m_lastPerpendicular; }

// =================================================================================================
std::queue<Protester::coord> *Protester::getLocations() { return &m_locations; }

void Protester::setLocations(std::queue<coord> &location) {
  m_locations = location;
}

std::queue<Protester::coord> *Protester::getPathOut() { return &m_pathOut; }

void Protester::setPathOut(std::queue<coord> &path) { m_pathOut = path; }

std::vector<Protester::coord> *Protester::getVisited() { return &m_visited; }

void Protester::setVisited(std::vector<coord> &visited) { m_visited = visited; }

// bool Protester::compareCoord (const coord &member, const coord &visited) {
//     return ((member.x == visited.x) && (member.y == visited.y));
// }

bool Protester::checkVisited(int x, int y) {
  std::vector<coord>::iterator it;
  for (it = m_visited.begin(); it < m_visited.end(); it++) {
    if (it->x == x && it->y == y) {
      return true; // in m_visited already
    }
  }
  return false;
}

// =================================================================================================
/*
void Protester::setLeave() {
    //
    std::queue<coord> path;
    coord curr;
    for (int i = this->getX(); i <= 60; i++) {
        curr.x = i;
        curr.y = this->getY();
        path.push(curr);
    }
    for (int j = this->getY(); j <= 60; j++) {
        curr.x = 60;
        curr.y = j;
        path.push(curr);
    }
    this->setPathOut(path);
    //
    //
    std::queue<coord> loc;
    std::queue<coord> path;
    std::vector<coord>* vis;
    coord curr;
    coord start;
    start.x = this->getX();
    start.y = this->getY();
    this->getLocations()->push(start);
    //loc.push(start);     // pushing the starting point
    // this->getWorld()->setEarthDiscovered(this->getX(), this->getY()); // set
start as discovered this->getVisited()->push_back(start);
    //while (!loc.empty()) {
    while (!this->getLocations()->empty()) {
        this->getWorld()->playSound(SOUND_FOUND_OIL);
        coord temp;
        curr = this->getLocations()->front();
        this->getLocations()->pop();
        this->moveTo(curr.x, curr.y);           // temp
xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx
        path.push(curr);    // add onto the path queue
        this->setPathOut(path);
        if (curr.x == 60 && curr.y == 60) {
            this->setPathOut(path);
            return; // done with path
        }
        if (this->getWorld()->positionClearLR(curr.x - 1, curr.y) &&
!this->getWorld()->inBoulderArea(curr.x - 1, curr.y)) { if (!checkVisited(curr.x
- 1, curr.y)) { temp.x = curr.x - 1; temp.y = curr.y;
                this->getLocations()->push(temp);
                vis = this->getVisited();
                vis->push_back(temp);
                this->setVisited(*vis);
            }
        }
        if (this->getWorld()->positionClearLR(curr.x + 4,curr.y) &&
!this->getWorld()->inBoulderArea(curr.x + 4, curr.y)) { if (!checkVisited(curr.x
+ 1, curr.y)) { temp.x = curr.x + 1; temp.y = curr.y;
                this->getLocations()->push(temp);
                vis = this->getVisited();
                vis->push_back(temp);
                this->setVisited(*vis);
            }
        }
        if (this->getWorld()->positionClearUD(curr.x, curr.y + 4) &&
!this->getWorld()->inBoulderArea(curr.x, curr.y + 4)) { if
(!checkVisited(curr.x, curr.y + 1)) { temp.x = curr.x; temp.y = curr.y + 1;
                this->getLocations()->push(temp);
                vis = this->getVisited();
                vis->push_back(temp);
                this->setVisited(*vis);
            }
        }
        if (this->getWorld()->positionClearUD(curr.x, curr.y - 1) &&
!this->getWorld()->inBoulderArea(curr.x, curr.y - 1)) {
            // if (!this->getWorld()->getEarthDiscovered(curr.x, curr.y - 1)) {
            if (!checkVisited(curr.x, curr.y - 1)) {
                temp.x = curr.x;
                temp.y = curr.y - 1;
                // loc.push(temp);
                // this->setLocations(loc);
                this->getLocations()->push(temp);
                // this->getWorld()->setEarthDiscovered(curr.x, curr.y - 1);
                // this->getVisited()->push_back(temp);
                vis = this->getVisited();
                vis->push_back(temp);
                this->setVisited(*vis);
            }
        }
    }
    //
}
*/
// =================================================================================================

double Protester::getUnitsFromTM() {
  int x = this->getX() - m_TM->getX();
  int y = this->getY() - m_TM->getY();
  int z = pow(x, 2) + pow(y, 2);
  double d = sqrt(z);
  return d;
}

bool Protester::getFacingTM() {
  if (this->getX() < m_TM->getX() && this->getDirection() == right) {
    return true;
  }
  if (this->getX() > m_TM->getX() && this->getDirection() == left) {
    return true;
  }
  if (this->getY() < m_TM->getY() && this->getDirection() == up) {
    return true;
  }
  if (this->getY() > m_TM->getY() && this->getDirection() == down) {
    return true;
  }
  return false;
}

bool Protester::lineOfSightTM() {
  return (this->getX() == this->getTM()->getX() ||
          this->getY() == this->getTM()->getY());
}

bool Protester::turnsTowardsTM() {
  if (this->getX() > this->getTM()->getX()) {
    if (this->checkPath(this->getTM()->getX(), this->getX(), this->getY(),
                        true)) {
      this->setDirection(left);
      this->moveTo(this->getX() - 1, this->getY());
      return true;
    }
  } else if (this->getX() < this->getTM()->getX()) {
    if (this->checkPath(this->getX(), this->getTM()->getX(), this->getY(),
                        true)) {
      this->setDirection(right);
      this->moveTo(this->getX() + 1, this->getY());
      return true;
    }
  } else if (this->getY() > this->getTM()->getY()) {
    if (this->checkPath(this->getTM()->getY(), this->getY(), this->getX(),
                        false)) {
      this->setDirection(down);
      this->moveTo(this->getX(), this->getY() - 1);
      return true;
    }
  } else if (this->getY() < this->getTM()->getY()) {
    if (this->checkPath(this->getY(), this->getTM()->getY(), this->getX(),
                        false)) {
      this->setDirection(up);
      this->moveTo(this->getX(), this->getY() + 1);
      return true;
    }
  }
  return false;
}

bool Protester::chooseDirection(Direction dir) {
  bool blocked = false;
  switch (dir) {
  case up:
    if (this->getY() != 60) {
      for (int i = 0; i < 4; i++) {
        if ((this->getWorld()->dirtExistsVisible(this->getX() + i,
                                                 this->getY() + 1)) ||
            this->getY() < 60) {
          blocked = true;
        }
      }
    }
    break;
  case down:
    if (this->getY() != 0) {
      for (int i = 0; i < 4; i++) {
        if ((this->getWorld()->dirtExistsVisible(this->getX() + i,
                                                 this->getY() - 1)) ||
            this->getY() < 0) {
          blocked = true;
        }
      }
    }
    break;
  case right:
    if (this->getX() != 60) {
      for (int i = 0; i < 4; i++) {
        if ((this->getWorld()->dirtExistsVisible(this->getX() + 1,
                                                 this->getY() + i)) ||
            this->getX() > 60) {
          blocked = true;
        }
      }
    }
    break;
  case left:
    if (this->getY() != 0) {
      for (int i = 0; i < 4; i++) {
        if ((this->getWorld()->dirtExistsVisible(this->getX() - 1,
                                                 this->getY() + i)) ||
            this->getX() < 0) {
          blocked = true;
        }
      }
    }
    break;
  default:
    break;
  }
  if (!blocked) {
    return true;
  }
  return false;
}

enum Actor::Direction Protester::checkTrapped(Direction dir) {
  if (dir == up || dir == down) {
    if (!this->getWorld()->positionClearLR(this->getX() + 4, this->getY()) &&
        !this->getWorld()->positionClearLR(this->getX() - 1, this->getY())) {
      if (dir == up) {
        if (!this->getWorld()->positionClearUD(this->getX(),
                                               this->getY() + 4)) {
          return down;
        }
      } else {
        if (!this->getWorld()->positionClearUD(this->getX(),
                                               this->getY() - 1)) {
          return up;
        }
      }
    }
  }
  if (dir == right || dir == left) {
    if (!this->getWorld()->positionClearUD(this->getX(), this->getY() + 4) &&
        !this->getWorld()->positionClearUD(this->getX(), this->getY() - 1)) {
      if (dir == right) {
        if (!this->getWorld()->positionClearLR(this->getX() + 4,
                                               this->getY())) {
          return left;
        }
      } else {
        if (!this->getWorld()->positionClearLR(this->getX() - 1,
                                               this->getY())) {
          return right;
        }
      }
    }
  }
  return none;
}

bool Protester::checkTMLR(int x, int y) {
  for (int i = 0; i < 4; i++) {
    if (this->getWorld()->inTMy(y + i)) {
      if (this->getWorld()->inTMx(x)) {
        return false;
      }
    }
  }
  return true;
}

bool Protester::checkTMUD(int x, int y, Direction dir) {
  for (int i = 0; i < 4; i++) {
    if (this->getWorld()->inTMx(x + i)) {
      if (this->getWorld()->inTMy(y)) {
        return false;
      }
    }
  }
  return true;
}

void Protester::protesterYells() {
  this->getWorld()->playSound(SOUND_PROTESTER_YELL);
  this->setLastShouted(0);
  this->getTM()->setHitPoints(this->getTM()->getHitPoints() - 2);
  if (this->getTM()->getHitPoints() < 1) {
    this->getTM()->setAlive(false);
    this->getWorld()->playSound(SOUND_PLAYER_GIVE_UP);
  }
}

bool Protester::checkPath(int start, int end, int same, bool changeX) {
  if (changeX == true) {
    for (int i = start; i < end; i++) {
      for (int j = 0; j < 4; j++) {
        if (this->getWorld()->dirtExistsVisible(i, same + j)) {
          return false;
        }
      }
    }
  } else {
    for (int i = start; i < end; i++) {
      for (int j = 0; j < 4; j++) {
        if (this->getWorld()->dirtExistsVisible(same + j, i)) {
          return false;
        }
      }
    }
  }
  return true;
}

Protester::~Protester() {}

RegularProtester::RegularProtester(StudentWorld &game, Tunnelman &TM)
    : Protester(TID_PROTESTER, game, TM) {
  this->setHitPoints(5);
}

void RegularProtester::doSomething() {
  // 1)
  if (this->getAlive() == false) {
    return;
  }
  // 2)
  if (this->getRestState() == true) {
    this->decRestTickCount();
    return;
  } else {
    this->setRestTickCount(this->getWaitTicks());
    this->setRestState(true);
  }
  // 3)
  if (this->getLeaveStatus() == true) {
    if (this->getX() == 60 && this->getY() == 60) {
      this->setAlive(false); // set to dead
    } else {
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
      // std::queue<coord>* where = this->getPathOut();
      coord place;
      std::cout << this->getX() << ',' << this->getY() << std::endl;
      std::cout << place.x << ',' << place.y << std::endl;
      std::cout << "-------------------------\n";
      if (!this->getPathOut()->empty()) { // why is it empty???
        place = this->getPathOut()->front();
        if (place.x > this->getX()) {
          this->setDirection(right);
        } else if (place.x < this->getX()) {
          this->setDirection(left);
        } else if (place.y > this->getY()) {
          this->setDirection(up);
        } else if (place.y > this->getY()) {
          this->setDirection(down);
        }
        this->moveTo(place.x, place.y);
        this->getPathOut()->pop();
        // this->setPathOut(*where);
        return;
      }
      this->moveTo(0, 0); // temp xxxxxxxxxxxxxxxx
      return;
      // ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
    }
  }
  // 4)
  bool yelled = false;
  if (this->getUnitsFromTM() <= 4 && this->getFacingTM()) {
    if (this->getLastShouted() > 15) {
      this->protesterYells();
      yelled = true;
      return;
    }
  }
  if (!yelled) {
    // increments the m_lastShouted when there is no shouting that tick
    this->setLastShouted(this->getLastShouted() + 1);
  }
  // 5)
  if ((this->getUnitsFromTM() > 4) && (this->lineOfSightTM())) {
    if (this->turnsTowardsTM()) {
      this->setMovesCurrDir(0);
      return;
    }
  }
  // 6)
  bool turned = false;
  this->setMovesCurrDir(this->getMovesCurrDir() - 1);
  if (this->getMovesCurrDir() <= 0) {
    // stops spazing out
    if (this->getFacingTM() && this->getUnitsFromTM() <= 4 &&
        (this->getX() == this->getTM()->getX() ||
         this->getY() == this->getTM()->getY())) {
      return;
    }
    bool decided = false;
    Direction randDir;
    // currDir & both perpendicular Dirs are blocked, go back opposite way
    randDir = this->checkTrapped(this->getDirection());
    if (randDir != none) {
      decided = true;
    }
    while (!decided) {
      randDir = Direction(rand() % 4 + 1);
      decided = this->chooseDirection(randDir);
    }
    // after decided is true
    this->setDirection(randDir);
    this->initMovesCurrDir();
  }
  // 7)
  else {
    Direction currDir = this->getDirection();
    Direction nextDir = none;
    switch (currDir) {
    case up:
    case down:
      if (this->getWorld()->positionClearLR(this->getX() + 4, this->getY()) ||
          this->getWorld()->positionClearLR(this->getX() - 1, this->getY())) {
        if (!this->getWorld()->positionClearLR(this->getX() + 4,
                                               this->getY())) {
          nextDir = left;
        } else if (!this->getWorld()->positionClearLR(this->getX() - 1,
                                                      this->getY())) {
          nextDir = right;
        } else {
          int num = rand() % 2;
          if (num == 0) {
            nextDir = left;
          } else {
            nextDir = right;
          }
        }
      }
      break;
    case right:
    case left:
      if (this->getWorld()->positionClearUD(this->getX(), this->getY() + 4) ||
          this->getWorld()->positionClearUD(this->getX(), this->getY() - 1)) {
        if (!this->getWorld()->positionClearUD(this->getX(),
                                               this->getY() + 4)) {
          nextDir = down;
        } else if (!this->getWorld()->positionClearUD(this->getX(),
                                                      this->getY() - 1)) {
          nextDir = up;
        } else {
          int num = rand() % 2;
          if (num == 0) {
            nextDir = down;
          } else {
            nextDir = up;
          }
        }
      }
      break;
    default:
      break;
    }
    if (nextDir != none) {
      if (this->getLastPerpendicular() > 200) {
        turned = true;
        this->setDirection(nextDir);
        this->setLastPerpendicular(0);
        this->initMovesCurrDir();
      }
    }
  }
  if (!turned) {
    this->setLastPerpendicular(this->getLastPerpendicular() + 1);
  }
  // 8)
  Direction currDir = this->getDirection();
  switch (currDir) {
  case up:
    if (this->getY() != 60 &&
        this->getWorld()->positionClearUD(this->getX(), this->getY() + 4)) {
      if (!this->getFacingTM() ||
          this->checkTMUD(this->getX(), this->getY() + 4,
                          up)) { // not facing TM or facing, checked = true
        if (this->getUnitsFromTM() >= 4) {
          if (!this->getWorld()->inBoulderArea(this->getX(),
                                               this->getY() + 1)) {
            this->moveTo(this->getX(), this->getY() + 1);
          } else {
            this->setMovesCurrDir(0);
          }
        }
      }
      return;
    }
    break;
  case down:
    if (this->getY() != 0 &&
        this->getWorld()->positionClearUD(this->getX(), this->getY() - 1)) {
      if (!this->getFacingTM() ||
          this->checkTMUD(this->getX(), this->getY() - 4, down)) {
        if (this->getUnitsFromTM() >= 4) {
          if (!this->getWorld()->inBoulderArea(this->getX(),
                                               this->getY() - 1)) {
            this->moveTo(this->getX(), this->getY() - 1);
          } else {
            this->setMovesCurrDir(0);
          }
        }
      }
      return;
    }
    break;
  case right:
    if (this->getX() != 60 &&
        this->getWorld()->positionClearLR(this->getX() + 4, this->getY())) {
      if (!this->getFacingTM() ||
          this->checkTMLR(this->getX() + 4, this->getY())) {
        if (this->getUnitsFromTM() >= 4) {
          if (!this->getWorld()->inBoulderArea(this->getX() + 1,
                                               this->getY())) {
            this->moveTo(this->getX() + 1, this->getY());
          } else {
            this->setMovesCurrDir(0);
          }
        }
      }
      return;
    }
    break;
  case left:
    if (this->getX() != 0 &&
        this->getWorld()->positionClearLR(this->getX() - 1, this->getY())) {
      if (!this->getFacingTM() ||
          this->checkTMLR(this->getX() - 4, this->getY())) {
        if (this->getUnitsFromTM() >= 4) {
          if (!this->getWorld()->inBoulderArea(this->getX() - 1,
                                               this->getY())) {
            this->moveTo(this->getX() - 1, this->getY());
          } else {
            this->setMovesCurrDir(0);
          }
        }
      }
      return;
    }
    break;
  default:
    break;
  }
  // 9)
  this->setMovesCurrDir(0);
}

void RegularProtester::annoy() {}

RegularProtester::~RegularProtester() {}
