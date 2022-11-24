# Docs
There are a lot of classes and tons of code so lets start with the simplest unit

## GraphObject
- Everything except the game world inherits from this
- it has this constructor signature
```cpp
GraphObject(int imageID, int startX, int startY, Direction dir = right, double size = 1.0, unsigned int depth = 0)
```
- **Important**: direction, size, and depth have a dedfault arguments right, 1.0, and 0 respectively

## Actor
- Inherits from GraphObject
- Serves as the parent to most game objects
- It has this constructor signature
```cpp
Actor(bool visible, int imageID, int startX, int startY, Direction dir, unsigned int depth, double size = 1.0);
```
- **Important**: new parameter `visible`. Required because some game objects are no immediately visible (ae. oil, gold). Sets visible in constructor. Also default value for `size` because MOST game objects have same size
- `doSomething` is a pure virtual function for the Actor class

## Earth
- Inherits from Actor
- This is the constructor signature
```cpp
Earth(int startX, int startY);
```
- only takes coordinates because all arguments required by Actor are the **same** for all Earth Objects. Think visibility, imageID, direction, size, and depth -> true, TID_EARTH, right, 3, 0.25
- The doSomething function simply returns since Earth doesn't do anything but has to be implemented because it's pure virtual in the parent

## Entity
- Inherits from Actor
- Serves as the parent class of Tunnelman and protesters
- This is the constructor signature
```cpp
Entity(int imageID, int startX, int startY);
```
- Only takes imageID, startX, startY because all other arguments required by the parent are the same for entities (ae. visibility=true, direction=right, depth=0). Size is 1 but not explicitly set here because that's the default in Actor
- Entities get a attribuite hitpoints. So also gets setter and getters for that.
- doSomething is also pure virtual here because what a Tunnelman does is different from protesters

## Tunnelman
- This is the player
- Inherits from Entity
- This is the constructor signature:
```cpp
Tunnelman(StudentWorld& game);
```
- When a Tunnelman digs, it must set the Earth there to invisible. That requires a StudentWorld reference (since that holds the earth)
- Since Tunnelman always start at (30, 60), always uses same image, and always start at same direction, these are parameters. They are instead set by the consturctor.
- Constructor set StudentWorld attribute, and hitpoints
- Tunnelman get new attributes
    - water, sonar, and gold (so also setters and getters)
- `doSomething`: 
    - if the player is alive and they entered a key
        - if already facing that direction
            - if not on an edge
                - try and dig
                - move to that location
        - otherwise simply look that way
- **IMPORTANT**: digging involves digging 4 blocks. If digging up or right I am checking 4 blocks to my side since TM is 4x4 but coords are always bottom left. Dig checks validity of the action

## StudentWorld
- **TODO**

# StudentWorld
- keep strack of earth on oil field, boulders, protesters, consumables, and tunnelman
- initializes game world at start of game, call doSomething method ono all actors, free's memory on game/level end and when an actor's state is dead
- `init`:
    - creates oil field, boulders, oil, and gold
    - must return DWSTATUS_CONTINUE_GAME
- `move`:
    - called 10-20 times a sec (ae. our game is 20 tick)
    - calls doSomething on all of the actors
    - **after calling doSomething free memory for any actor that is dead**
- `cleanup`:
    - free all memory
    - 

# Terrain (parent of Earth and Boulder)
- TID_{name}
- x, y
- IS_ALIVE
- set visibility with GraphObject->setVisible()
- Can't be annoyed
## Boulder
- starts stable
- faces = down
- depth = 1
- size = 1.0
- doSomething checks if alive. if no return
    - if stable, check if earth in 4 squares below it (in the oil field, cols). if no earth below, wait 30 ticks
    - when times up, fall and play SOUND_FALLING_ROCK
    - if falling, move down 1 square per tick until it reaches bottom of field, boulder, or earth. Set state to dead
    - while falling if within 3 blocks of an entity, +100 annoyance except tunnelman. he folds

- tunnel man start at (30, 60), (col, row), (center of window, top of window)

- look into setGameStatText. Spec says pass just pass is a string of the form

```python
f"Lvl: {self.level} Lives {self.lives} Hlth: {self.health}% Wtr: {self.water} Gld: {self.gold} Oil Left: {self.oil} Sonar: {self.sonar} Scr: {self.score}"
```
- but that function does nothing. GameWorld.h line 29

# New rules
- non-entitys can't spawn within 6 units of eachother
- must spawn actors randomly
- nuggets and oil between (0, 0) and (60, 56) (COL, ROW)
- boulders between (0, 20) and (60, 56) **PAGE 17**
- **Problem**: I need to be able to randomly place objects, but as levels increase, more
non-entitys will be spawned so simply generating a random x, y and checking against all non-entitys
is no good. Need to efficienlty map the space and randomly select a valid node, taking on that nodes
x and y

```cpp
  //  // used in naive approach to create valid NE location
  //  std::random_device rd;                            // random num from
  //  hardware std::mt19937 gen(rd());                           // seed the
  //  generator std::uniform_int_distribution<> col_dist(0, 60);  // set range,
  //  inclusive std::uniform_int_distribution<> row_dist(20, 56); // could be
  //  improved, only bool distance_cond_met = true; bool randomly_placed =
  //  false; vector<vector<int>> invalid_locs;
  //  // place boulders
  //  int num_boulders = min((this->getLevel() / 2) + 2, MAX_BOULDERS);
  //  // naive approach so I can at least display them + create logic to clear
  //  earth for (int i = 0; i < num_boulders; i++) { // for num_boulders
  //    while (!randomly_placed) {             // until a valid coord is found
  //      int potential_x, potential_y;        // randomly generate x and y
  //      potential_x = col_dist(gen);
  //      potential_y = row_dist(gen);
  //      std::cout << potential_x << endl;
  //      // changed to 29 - 3. i think this will keep it from clipping the
  //      shaft if ((potential_x < (29 - 3)) || (potential_x > 34)) { // not at
  //      shaft
  //        // i think this cond ius gargabe. dist produces that so...
  //        if ((potential_y > 19) &&
  //            (potential_y < 57)) { // row [20, 56] like spec
  //          vector<int> temp = {potential_x, potential_y};
  //          if (find(invalid_locs.begin(), invalid_locs.end(), temp) ==
  //              invalid_locs.end()) { // location is not taken
  //            for (auto boulder :
  //                 this->boulders) { // check distacne against each NE
  //              if (!(validSpawn(boulder->getX(), boulder->getY(),
  //              potential_x,
  //                               potential_y))) { // if violate distnace rule
  //                distance_cond_met = false;
  //                break;
  //              }
  //            }
  //            if (distance_cond_met) {
  //              this->boulders.push_back(std::move(
  //                  new Boulder(true, potential_x, potential_y))); // save it
  //              randomly_placed = true;                            // break
  //              loop
  //            }
  //          }
  //        }
  //      }
  //    }
  //    distance_cond_met = true;
  //    randomly_placed = false; // reset for next iter
  //  }
  //
  //  // int num_nuggs = max((5 - this->getLevel()) / 2, MAX_GOLD_NUGGETS);
  //  // int num_oil = min(2 + this->getLevel(), MAX_OIL_BARRELS);
```