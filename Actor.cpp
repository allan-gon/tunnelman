#include "Actor.h"
#include "StudentWorld.h"

// Students:  Add code to this file (if you wish), Actor.h, StudentWorld.h, and StudentWorld.cpp

Actor::Actor(bool visible, int imageID, int startX, int startY, Direction dir, double size, unsigned int depth):
GraphObject(imageID, startX, startY, dir, size, depth){this->setVisible(visible);}

bool Actor::getAlive(){return this->is_alive;}

void Actor::setAlive(bool alive){this->is_alive = alive;}

Earth::Earth(bool visible, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth):
Actor(visible, imageID, startX, startY, dir, size, depth){}

Boulder::Boulder(bool visible, int startX, int startY, int imageID, Direction dir, double size, unsigned int depth):
Actor(visible, imageID, startX, startY, dir, size, depth){}

void Boulder::doSomething(){
    if (this->getAlive()){
        if (this->is_stable){
            
        }
    } return;
}