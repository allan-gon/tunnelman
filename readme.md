# StudentWorld
- keep strack of earth on oil field, boulders, protesters, consumables, and tunnelman
- initializes game world at start of game, call doSomething method ono all actors, free's memory on game/level end and when an actor's state is dead
- `init`:
    - creates oil field, boulders, oil, and gold
    - must return DWSTATUS_CONTINUE_GAME
- `move`:
    - called 10-20 times a sec (ae. our game is 20 tick)
    - calls doSomething on all of the actors
    - after calling doSomething free memory for any actor that is dead
- `cleanup`:
    - free all memory
    - 

# Terrain (parent of Earth and Boulder)
- TID_{name}
- x, y
- IS_ALIVE
- set visibility with GraphObject->setVisible()
- Can't be annoyed
## Earth
- faces right
- depth = 3
- size = .25
- doSomething() doesn't do anything
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
- nuggets and oil between (0, 0) and (60, 56) (ROW, COL)
- boulders between (0, 20) and (60, 56)
- **Problem**: I need to be able to randomly place objects, but as levels increase, more
non-entitys will be spawned so simply generating a random x, y and checking against all non-entitys
is no good. Need to efficienlty map the space and randomly select a valid node, taking on that nodes
x and y

- `Conceptual Solution`: if I can map the space, then I can say what locations are open and randomly select one of them

- `Problem`: I have to randomly place non-entities (NE) such that none of them are with 6 units of eachother. With a small amount of NE this is easy. A naive approach would be to randomly select x and y coords then check against all existing NE. If the condition of 6 units is not violated then I can use the coords otherwise regenerate the coords. What if I get REALLY unlucky? My game could pause for minutes while trying to spawn NE. This is very unlikely with a small number of NE but as number of NE grows likelyhood of randomly generating an invalid coord increases. I need an efficient way to mapp the space return all possible locations and randomly select one fo those.