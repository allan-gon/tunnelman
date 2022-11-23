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
  int key;
  // if player is alive and a key was pressed
  if (this->getAlive() && this->getWorld()->getKey(key)) {
    switch (key) {
    case KEY_PRESS_LEFT:
      if (this->getDirection() == left) { // if already facing in that direction
        if (this->getX() != 0) {          // if not on edge
          if (this->getWorld()->dirtExists(
                  this->getX() - 1, this->getY())) { // if there's dirt there
            this->getWorld()->digDirtLR(this->getX() - 1, this->getY());
          }
          this->moveTo(this->getX() - 1, this->getY());
        }
      } else {
        this->setDirection(left);
      }
      break;
    case KEY_PRESS_RIGHT:
      if (this->getDirection() == right) {
        if (this->getX() != 60) {
          if (this->getWorld()->dirtExists(this->getX() + 1, this->getY())) {
            this->getWorld()->digDirtLR(this->getX() + 4, this->getY());
          }
          this->moveTo(this->getX() + 1, this->getY());
        }
      } else {
        this->setDirection(right);
      }
      break;
    case KEY_PRESS_DOWN:
      if (this->getDirection() == down) {
        if (this->getY() != 0) {
          if (this->getWorld()->dirtExists(this->getX(), this->getY() - 1)) {
            this->getWorld()->digDirtUD(this->getX(), this->getY() - 1);
          }
          this->moveTo(this->getX(), this->getY() - 1);
        }
      } else {
        this->setDirection(down);
      }
      break;
    case KEY_PRESS_UP:
      if (this->getDirection() == up) {
        if (this->getY() != 60) {
          if (this->getWorld()->dirtExists(this->getX(), this->getY() + 1)) {
            if (this->getY() < 60) { // padding since tunnel man takes a 4x4
              this->getWorld()->digDirtUD(this->getX(), this->getY() + 4);
            }
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

Tunnelman::~Tunnelman(){};
