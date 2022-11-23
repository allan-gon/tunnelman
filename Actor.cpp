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

// void Tunnelman::doSomething() {
//   if (this->getAlive() == false) {
//     return;
//   } else {
//     int ch;
//     if (getWorld()->getKey(ch) ==
//         true) { // checks if the user hit a key during the tick
//       switch (ch) {
//       case KEY_PRESS_LEFT:
//         // checks if TM is facting the same direction as the key
//         if (this->getDirection() == left) {
//           // prevents TM from going out of bounds
//           if (this->getX() != 0) {
//             // checks if there is dirt in the space that TM wants to move to
//             if (getWorld()->dirtExists(this->getX() - 1, this->getY()) ==
//                 true) {
//               // needs to set earth to not visible
//               getWorld()->digDirtLR(this->getX() - 1, this->getY());
//             }
//             moveTo(this->getX() - 1, this->getY());
//           }
//         } else {
//           this->setDirection(left);
//         }
//         break;
//       case KEY_PRESS_RIGHT:
//         if (this->getDirection() == right) {
//           if (this->getX() != 60) {
//             if (getWorld()->dirtExists(this->getX() + 1, this->getY()) ==
//                 true) {
//               getWorld()->digDirtLR(this->getX() + 4, this->getY());
//             }
//             moveTo(this->getX() + 1, this->getY());
//           }
//         } else {
//           this->setDirection(right);
//         }
//         break;
//       case KEY_PRESS_UP:
//         if (this->getDirection() == up) {
//           if (this->getY() != 60) {
//             if (getWorld()->dirtExists(this->getX(), this->getY() + 1) ==
//                 true) {
//               if (this->getY() < 56) {
//                 getWorld()->digDirtUD(this->getX(), this->getY() + 4);
//               }
//             }
//             moveTo(this->getX(), this->getY() + 1);
//           }
//         } else {
//           this->setDirection(up);
//         }
//         break;
//       case KEY_PRESS_DOWN:
//         if (this->getDirection() == down) {
//           if (this->getY() != 0) {
//             if (getWorld()->dirtExists(this->getX(), this->getY() - 1) ==
//                 true) {
//               getWorld()->digDirtUD(this->getX(), this->getY() - 1);
//             }
//             moveTo(this->getX(), this->getY() - 1);
//           }
//         } else {
//           this->setDirection(down);
//         }
//         break;
//       }
//     }
//   }
// };

bool onEdge(int a, int b){
  return ((a == 0) || (a == 63) || (b == 0) || (b == 63));
}

void Tunnelman::doSomething(){
  int key;
  if (this->getAlive() && this->getWorld()->getKey(key)){ // if player is alive and a key was pressed
    switch (key){
    case KEY_PRESS_LEFT:
      if (this->getDirection() == left){
        this->moveTo(this->getX() - 1, this->getY());
      } else {this->setDirection(left);}
      break;
    case KEY_PRESS_RIGHT:
      if (this->getDirection() == right){
        this->moveTo(this->getX() + 1, this->getY());
      } else {this->setDirection(right);}
      break;
    case KEY_PRESS_DOWN:
      if (this->getDirection() == down){
        this->moveTo(this->getX(), this->getY() - 1);
      } else {this->setDirection(down);}
      break;
    case KEY_PRESS_UP:
      if (this->getDirection() == up){
        this->moveTo(this->getX(), this->getY() + 1);
      } else {this->setDirection(up);}
      break;
    default:
      break;
    }
  }
}

Tunnelman::~Tunnelman(){};
