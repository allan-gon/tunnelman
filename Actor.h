#ifndef ACTOR_H_
#define ACTOR_H_

#include "GraphObject.h"

class Actor : public GraphObject {
public:
  Actor(bool visible, int imageID, int startX, int startY, Direction dir,
        double size, unsigned int depth);
  virtual void doSomething() = 0;
  bool getAlive();
  void setAlive(bool alive);

private:
  bool is_alive = true;
};

class Earth : public Actor {
public:
  Earth(bool visible, int startX, int startY, int imageID = TID_EARTH,
        Direction dir = right, double size = 0.25, unsigned int depth = 3);
  virtual void doSomething() {
    return;
  }; // actually does nothing but needed because earth and boulder share a
     // parent which has this func as pure virtual
};

class Boulder : public Actor {
public:
  Boulder(bool visible, int startX, int startY, int imageID = TID_BOULDER,
          Direction dir = down, double size = 1.0, unsigned int depth = 1);
  virtual void doSomething();

private:
  bool is_stable = true;
};

#endif // ACTOR_H_
