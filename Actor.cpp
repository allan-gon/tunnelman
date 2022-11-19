#include "Actor.h"

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
    : Actor(visible, imageID, startX, startY, dir, depth){};

Entity::~Entity(){};

Tunnelman::~Tunnelman(){};