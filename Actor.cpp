#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and
// StudentWorld.cpp

Actor::Actor(bool visible, int imageID, int startX, int startY, Direction dir,
             unsigned int depth, double size)
    : GraphObject(imageID, startX, startY, dir, size, depth) {
  this->setVisible(visible);
}

bool Actor::getAlive() { return this->is_alive; }

void Actor::setAlive(bool alive) { this->is_alive = alive; }

Earth::Earth(bool visible, int startX, int startY, int imageID, Direction dir,
             double size, unsigned int depth)
    : Actor(visible, imageID, startX, startY, dir, depth, size) {}

Boulder::Boulder(bool visible, int startX, int startY, int imageID,
                 Direction dir, unsigned int depth)
    : Actor(visible, imageID, startX, startY, dir, depth) {}

void Boulder::doSomething() {
  //     if (this->getAlive()){
  //         if (this->is_stable){

  //         }
  //     } return;
}

Entity::Entity(int imageID, int startX, int startY, Direction dir,
               unsigned int depth, bool visible)
    : Actor(visible, imageID, startX, startY, dir, depth) {}

void Entity::setHitPoints(int hitPoints) { this->m_hitPoints = hitPoints; }

int Entity::getHitPoints() { return this->m_hitPoints; }

void Entity::doSomething() { return; }

Entity::~Entity() {}

// added the StudentWorld address & initializes the member variable
Tunnelman::Tunnelman(StudentWorld& game, int imageID, int startX, int startY, Direction dir)
    : Entity(imageID, startX, startY, dir) , m_game(&game) {
    this->setHitPoints(10);
}

// added to get access to getKey()
StudentWorld* Tunnelman::getWorld() { return m_game; }

void Tunnelman::setWaterUnits(int waterUnits) { this->m_waterUnits = waterUnits; }

int Tunnelman::getWaterUnits() { return this->m_waterUnits; }

void Tunnelman::setSonarCharge(int sonarCharge) { this->m_sonarCharge = sonarCharge; }

int Tunnelman::getSonarCharge() { return this->m_sonarCharge; }

void Tunnelman::setGold(int gold) { this->m_gold = gold; }

int Tunnelman::getGold() { return this->m_gold; }

void Tunnelman::doSomething() {
    if (this->getAlive() == false) {
        return;
    }
    else {
        int ch;
        if (getWorld()->getKey(ch) == true) {    // checks if the user hit a key during the tick
            switch (ch) {
                case KEY_PRESS_LEFT:
                    // checks if TM is facting the same direction as the key
                    if (this->getDirection() == left) {
                        if (this->getX() != 0) {
                            moveTo(this->getX() - 1, this->getY());
                        }
                    }
                    else {
                        this->setDirection(left);
                    }
                    break;
                case KEY_PRESS_RIGHT:
                    if (this->getDirection() == right) {
                        if (this->getX() != 60) {
                            moveTo(this->getX() + 1, this->getY());
                        }
                    }
                    else {
                        this->setDirection(right);
                    }
                    break;
                case KEY_PRESS_UP:
                    if (this->getDirection() == up) {
                        if (this->getY() != 60) {
                            moveTo(this->getX(), this->getY() + 1);
                        }
                    }
                    else {
                        this->setDirection(up);
                    }
                    break;
                case KEY_PRESS_DOWN:
                    if (this->getDirection() == down) {
                        if (this->getY() != 0) {
                            moveTo(this->getX(), this->getY() - 1);
                        }
                    }
                    else {
                        this->setDirection(down);
                    }
                    break;
            }
        }
    }
};

        Tunnelman::~Tunnelman() {};
