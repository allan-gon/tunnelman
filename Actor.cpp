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

void Entity::takeDamage(int amount) {
  this->m_hitPoints = this->m_hitPoints - amount;
  if (this->m_hitPoints <= 0) {
    this->setAlive(false);
  }
}

Entity::~Entity() {}

// added the StudentWorld address & initializes the member variable
Tunnelman::Tunnelman(StudentWorld &game)
    : Entity(TID_PLAYER, 30, 60, right, game) {
  this->setHitPoints(10);
}

void Tunnelman::incWater5() { this->m_waterUnits = this->m_waterUnits + 5; }

void Tunnelman::decWater() { this->m_waterUnits--; }

int Tunnelman::getWaterUnits() { return this->m_waterUnits; }

void Tunnelman::incSonar() { this->m_sonarCharge++; }

void Tunnelman::decSonar() { this->m_sonarCharge--; }

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
          this->getWorld()->digDirtLR(this->getX() - 1, this->getY());
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
          this->getWorld()->digDirtLR(this->getX() + 4, this->getY());
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
          this->getWorld()->digDirtUD(this->getX(), this->getY() - 1);
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
            this->getWorld()->digDirtUD(this->getX(), this->getY() + 4);
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
      if (this->getWaterUnits() > 0) {
        this->decWater();

        Direction dir;
        int x, y;
        if (this->getX() == 60 && this->getDirection() == right) {
          break;
        } else if (this->getX() == 0 && this->getDirection() == left) {
          break;
        } else if (this->getY() == 60 && this->getDirection() == up) {
          break;
        } else if (this->getY() == 0 && this->getDirection() == down) {
          break;
        } else if (this->getWorld()->dirtObstructs(this) ||
                   this->getWorld()->boulderObstructs(this)) {
          break;
        } else {
          if (this->getDirection() == up) {
            x = this->getX();
            y = this->getY() + 4;
            dir = up;
          } else if (this->getDirection() == down) {
            x = this->getX();
            y = this->getY() - 4;
            dir = down;
          } else if (this->getDirection() == left) {
            x = this->getX() - 4;
            y = this->getY();
            dir = left;
          } else if (this->getDirection() == right) {
            x = this->getX() + 4;
            y = this->getY();
            dir = right;
          }

          this->getWorld()->getActors().push_back(
              std::move(new Squirt(x, y, dir, *this->getWorld())));
        }
      }
      break;
      // =================================================================================================
    case 'z':
    case 'Z':
      if (this->getSonarCharge() > 0) {
        this->decSonar();
        for (auto actor : this->getWorld()->getActors()) {
          if ((actor->getID() == TID_BARREL) || (actor->getID() == TID_GOLD)) {
            if (inRange(this->getX(), this->getY(), actor->getX(),
                        actor->getY(), 12)) {
              actor->setVisible(true);
            }
          }
        }
      }
      break;
    default:
      break;
    }
  }
}

Tunnelman::~Tunnelman() {}

Protester::Protester(int imageID, StudentWorld &game, Tunnelman &TM)
    : Entity(imageID, 60, 60, left, game), m_TM(&TM) {
  this->initMovesCurrDir();
}

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

std::queue<Protester::coord> *Protester::getPathOut() { return &m_pathOut; }
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
      // algorithm to move the protester 1 step towards the exit & return
      //            std::queue<coord>* locPtr = getLocations();
      //            coord curr;
      //            curr.x = this->getX();
      //            curr.y = this->getY();
      //            locPtr->push(curr);
      //            this->getWorld()->setEarthDiscovered(this->getX(),
      //            this->getY());
      //
      //            if (locPtr->empty()) {
      //
      //            }
      //
      // add all values to the locations queue
      // pop values from the location and add to the path queue?
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
                          up)) { // not facing TM or facing, but checked = true
        if (this->getUnitsFromTM() >= 4) {
          this->moveTo(this->getX(), this->getY() + 1);
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
          this->moveTo(this->getX(), this->getY() - 1);
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
          this->moveTo(this->getX() + 1, this->getY());
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
          this->moveTo(this->getX() - 1, this->getY());
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

RegularProtester::~RegularProtester() {}

// visible should be false
OilBarrel::OilBarrel(int x, int y, StudentWorld &world)
    : Actor(false, TID_BARREL, x, y, right, 2), m_world(&world) {}

void OilBarrel::doSomething() {
  if (this->getAlive()) {
    if (!this->isVisible()) {
      if (inRange(this->getX(), this->getY(),
                  this->getWorld()->getPlayer()->getX(),
                  this->getWorld()->getPlayer()->getY(), 4)) {
        this->setVisible(true);
      }
    } else {
      if (inRange(this->getX(), this->getY(),
                  this->getWorld()->getPlayer()->getX(),
                  this->getWorld()->getPlayer()->getY(), 3)) {
        this->setAlive(false);
        this->getWorld()->playSound(SOUND_FOUND_OIL);
        this->getWorld()->increaseScore(1000);
        this->getWorld()->decBarrels();
      }
    }
  }
}

StudentWorld *OilBarrel::getWorld() { return this->m_world; }

OilBarrel::~OilBarrel() {}

Sonar::Sonar(StudentWorld &world)
    : Actor(true, TID_SONAR, 0, 60, right, 2), m_world(&world) {}

void Sonar::doSomething() {
  if (this->getAlive()) {
    if (this->ticks_existed == this->getWorld()->getTicks()) {
      this->setAlive(false);
    } else if (inRange(this->getX(), this->getY(),
                       this->getWorld()->getPlayer()->getX(),
                       this->getWorld()->getPlayer()->getY(), 3)) {
      this->setAlive(false);
      this->getWorld()->getPlayer()->incSonar();
      this->getWorld()->playSound(SOUND_GOT_GOODIE);
      this->getWorld()->increaseScore(75);
    }
    this->ticks_existed++;
  }
}
Sonar::~Sonar() {}

StudentWorld *Sonar::getWorld() { return this->m_world; }

WaterPool::WaterPool(int x, int y, StudentWorld &world)
    : Actor(true, TID_WATER_POOL, x, y, right, 2), m_world(&world) {}

void WaterPool::doSomething() {
  if (this->getAlive()) {

    if (this->ticks_existed == this->getWorld()->getTicks()) {
      this->setAlive(false);
    } else if (inRange(this->getX(), this->getY(),
                       this->getWorld()->getPlayer()->getX(),
                       this->getWorld()->getPlayer()->getY(), 3)) {
      this->setAlive(false);
      this->getWorld()->playSound(SOUND_GOT_GOODIE);
      this->getWorld()->getPlayer()->incWater5();
      this->getWorld()->increaseScore(100);
    }
    this->ticks_existed++;
  }
}

StudentWorld *WaterPool::getWorld() { return this->m_world; }

WaterPool::~WaterPool() {}

Squirt::Squirt(int x, int y, Direction dir, StudentWorld &world)
    : Actor(true, TID_WATER_SPURT, x, y, dir, 1), m_world(&world) {}

void Squirt::doSomething() {
  if (this->getAlive()) {
    if (this->getTicks() == 4) {
      this->setAlive(false);
    } else if (this->getWorld()->squirtAnnoyActors(this->getX(),
                                                   this->getY())) {
      this->setAlive(false);
    } else if (this->getWorld()->boulderObstructs(this) ||
               this->getWorld()->dirtObstructs(this)) {
      this->setAlive(false);
    } else {
      int x_mod = 0;
      int y_mod = 0;
      if (this->getDirection() == up) {
        y_mod++;
      } else if (this->getDirection() == down) {
        y_mod--;
      } else if (this->getDirection() == right) {
        x_mod++;
      } else if (this->getDirection() == left) {
        x_mod--;
      }

      this->moveTo(this->getX() + x_mod, this->getY() + y_mod);
    }
    this->incTicks();
  }
}

int Squirt::getTicks() { return this->ticks_alive; }

void Squirt::incTicks() { this->ticks_alive++; }

StudentWorld *Squirt::getWorld() { return this->m_world; }

Squirt::~Squirt() {}
 