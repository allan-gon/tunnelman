#include "Actor.h"
#include "StudentWorld.h"

Actor::Actor(bool visible, int imageID, int startX, int startY, Direction dir,
             unsigned int depth, double size)
    : GraphObject(imageID, startX, startY, dir, size, depth) {
  this->setVisible(visible);
}

bool Actor::getAlive() { return this->is_alive; }

void Actor::setAlive(bool alive) { this->is_alive = alive; }

Earth::Earth(int startX, int startY)
    : Actor(true, TID_EARTH, startX, startY, right, 3, 0.25) {}

// Boulder::Boulder(bool visible, int startX, int startY, int imageID,
//                  Direction dir, unsigned int depth)
//     : Actor(visible, imageID, startX, startY, dir, depth) {}

// void Boulder::doSomething() {};

Entity::Entity(int imageID, int startX, int startY, Direction dir, StudentWorld &game)
    : Actor(true, imageID, startX, startY, dir, 0), m_game(&game) {}

void Entity::setHitPoints(int hitPoints) { this->m_hitPoints = hitPoints; }

int Entity::getHitPoints() { return this->m_hitPoints; }

// getter & setter m_game Entity class
void Entity::setWorld(StudentWorld &game) { m_game = &game; }
StudentWorld *Entity::getWorld() { return m_game; }

void Entity::doSomething() { return; }

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
          this->getWorld()->digDirtLR(this->getX() - 1, this->getY());
          this->moveTo(this->getX() - 1, this->getY());
        }
      } else {
        this->setDirection(left);
      }
      break;
    case KEY_PRESS_RIGHT:
      if (this->getDirection() == right) {
        if (this->getX() != 60) {
          this->getWorld()->digDirtLR(this->getX() + 4, this->getY());
          this->moveTo(this->getX() + 1, this->getY());
        }
      } else {
        this->setDirection(right);
      }
      break;
    case KEY_PRESS_DOWN:
      if (this->getDirection() == down) {
        if (this->getY() != 0) {
          this->getWorld()->digDirtUD(this->getX(), this->getY() - 1);
          this->moveTo(this->getX(), this->getY() - 1);
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
          this->moveTo(this->getX(), this->getY() + 1);
        }
      } else {
        this->setDirection(up);
      }
      break;
    default:
      break;
    }
  }
}

Tunnelman::~Tunnelman() {}

Protester::Protester(int imageID, StudentWorld &game, Tunnelman &TM)
: Entity(imageID, 60, 60, left, game), m_TM(&TM) {}

void Protester::setLeaveStatus(bool leaveOilField) { m_leaveOilField = leaveOilField; }

bool Protester::getLeaveStatus() { return m_leaveOilField; }

void Protester::setWaitTicks(int numWait) { m_ticksToWait = numWait; }
int Protester::getWaitTicks() { return m_ticksToWait; }

void Protester::setRestState(bool restState) { m_restState = restState; }
bool Protester::getRestState() { return m_restState; }

void Protester::setRestTickCount(int numRestTick) { m_restTickCount = numRestTick; }
int Protester::getRestTickCount() { return m_restTickCount; }

void Protester::decRestTickCount() {            // added
    this->setRestTickCount(this->getRestTickCount() - 1);
    if (this->getRestTickCount() == 0) {
        this->setRestState(false);  // restTickCount = 0, restState = false (does something)
    }
}

void Protester::setMovesCurrDir(int numCurrDir) { m_numMoveCurrDir = numCurrDir; }
int Protester::getMovesCurrDir() { return m_numMoveCurrDir; }

void Protester::setTM(Tunnelman &TM) { m_TM = &TM; }
Tunnelman* Protester::getTM() { return m_TM; }

void Protester::setLastShouted(int numLastShouted) { m_lastShouted = numLastShouted; }
int Protester::getLastShouted() {return m_lastShouted; }

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

Protester::~Protester() {}

RegularProtester::RegularProtester(StudentWorld &game, Tunnelman &TM)
: Protester(TID_PROTESTER, game, TM) {
    this->setHitPoints(5);
}

void RegularProtester::doSomething() {
    // check alive
    if (this->getAlive() == false) { return; }      // checks alive
    if (this->getRestState() == true) {             // checks rest state
        // decrement restTickWait & return
        this->decRestTickCount();
        return;
    }
    if (this->getLeaveStatus() == true) {
        if (this->getX() == 60 && this->getY() == 60) {
            this->setAlive(false);      // set to dead
        }
        else {
            // algorithm to move the protester 1 step towards the exit & return
        }
    }
    if (this->getUnitsFromTM() < 4 && this->getFacingTM()) {
        // check that has not shouted within the last 15 non-resting ticks
        if (this->getLastShouted() > 15) {
            // shout & reduce TM hitpoints by 2 & updates last shouted data
            this->getWorld()->playSound(SOUND_PROTESTER_YELL);
            this->setLastShouted(0);
            this->getTM()->setHitPoints(this->getTM()->getHitPoints() - 2);
            // checks if TM dies
            if (this->getTM()->getHitPoints() < 1) {
                this->getTM()->setAlive(false);
            }
            return;
        }
    }
    // else {          // temp
        if (this->getDirection() == left && this->getX() != 0) {
            this->moveTo(this->getX() - 1, this->getY());       // temp
        }
        else if (this->getDirection() == left && this->getX() == 0) {
            this-> setDirection(right);
        }
        else if (this->getDirection() == right && this->getX() == 60) {
            this->setDirection(left);
        }
        else {
            this->moveTo(this->getX() + 1, this->getY());
        }
        
    // increments the m_lastShouted when there is no shouting that tick
        this->setLastShouted(this->getLastShouted() + 1);
    // resets the restTickCount & restState
        this->setRestTickCount(this->getWaitTicks());
        this->setRestState(true);
    // }
}

RegularProtester::~RegularProtester() {}
