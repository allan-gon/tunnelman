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

Boulder::Boulder(int startX, int startY, StudentWorld &world)
    : Actor(true, TID_BOULDER, startX, startY, down, 1), m_world(&world) {}

// State Boulder::getState() { return this->m_state; }

void Boulder::setState(State state) { this->m_state = state; }

StudentWorld *Boulder::getWorld() { return this->m_world; }

void Boulder::doSomething() {
  // TODO: missing, boulder does damage to entities
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
      if ((this->getY() > 0) && !this->getWorld()->boulderExists(this) &&
          !this->getWorld()->dirtBelow(this->getX(), this->getY())) {
        this->getWorld()->boulderAnnoyActors(this->getX(), this->getY());
        this->moveTo(this->getX(), this->getY() - 1); // move down 1 row
      } else {
        this->setState(dead);
      }
    } else if (this->getState() == dead) {
      this->setVisible(false);
    }
  }
}

Entity::Entity(int imageID, int startX, int startY)
    : Actor(true, imageID, startX, startY, right, 0) {}

void Entity::setHitPoints(int hitPoints) { this->m_hitPoints = hitPoints; }

int Entity::getHitPoints() { return this->m_hitPoints; }

void Entity::doSomething() { return; }

Entity::~Entity() {}

// added the StudentWorld address & initializes the member variable
Tunnelman::Tunnelman(StudentWorld &game)
    : Entity(TID_PLAYER, 30, 60), m_game(&game) {
  this->setHitPoints(10);
}

// added to get access to getKey()
StudentWorld *Tunnelman::getWorld() { return m_game; }

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
  // TODO: don't move if boulder
  int key;
  // if player is alive and a key was pressed
  if (this->getAlive() && this->getWorld()->getKey(key)) {
    switch (key) {
    case KEY_PRESS_LEFT:
      if (this->getDirection() == left) { // if already facing in that direction
        if (this->getX() != 0) {          // if not on edge
          this->getWorld()->digDirtLR(this->getX() - 1, this->getY());
          if (!this->getWorld()->boulderExists(this)) {
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
          if (!this->getWorld()->boulderExists(this)) {
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
          if (!this->getWorld()->boulderExists(this)) { // was boulderExists
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
          if (!this->getWorld()->boulderExists(this)) {
            this->moveTo(this->getX(), this->getY() + 1);
          }
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

void Tunnelman::annoy() {
  // possibly should increase annoyance
  this->setHitPoints(this->getHitPoints() - 100);
}

Tunnelman::~Tunnelman(){};
