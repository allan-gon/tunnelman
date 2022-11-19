#include "GameConstants.h"
#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init(){
	// initializes all game object when game start, or level resets/changes
	
	bool visible; // variable used to assign an Earth block's visibility based off location
	// initialize field of Earth objects
	for (int row = 0; row < VIEW_HEIGHT; row++){
		for (int col = 0; col < VIEW_WIDTH; col++){
			if (row < 4){ // if bottom 4 rows
				visible = true;
			} else if (row > 59){ // if top 4 rows
				visible = false;
			} else if ((col > 29) && (col < 34)){
				visible = false;
			} else {visible = true;};
			this->field[col][row] = std::move(new Earth(visible, col, row));
			// for now every square is shown
		}
	}
	return GWSTATUS_CONTINUE_GAME;
}

int StudentWorld::move(){
	// right now just returns a good status so i can see stuff on screen
	return GWSTATUS_CONTINUE_GAME;
}

void StudentWorld::cleanUp(){
	// free's any memory that was allocated to actors
	// and also fixes dangling pointer
	
	// frees memory from Earth object
	for (int row = 0; row < VIEW_HEIGHT; row++){
		for (int col = 0; col < VIEW_WIDTH; col++){
			delete this->field[col][row];
			this->field[col][row] = nullptr;
		}
	}
}