#include "GameConstants.h"
#include "StudentWorld.h"
#include "Actor.h"
#include <string>
using namespace std;

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

bool validSpawn(int x1, int y1, int x2, int y2){
	// returns whether or not the distance between point 1
	// and point 2 is > 6. Helper for StudentWorld::init
	return sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2)) > 6;
}

// Students:  Add code to this file (if you wish), StudentWorld.h, Actor.h and Actor.cpp

int StudentWorld::init(){
	// TODO: actually only need 64 col, 60 row. So allocating more than needed.
	// Possibly also alloc more than needed for shaft? Though this is true,
	// not allocating those would mean instead of setting visibility, only
	// alloc when condition 1 and else. "row < VIEW_HEIGHT" is wrong since 64, 60
	// could hard code 60. Not into that idea. Could simply say VIEW_WIDTH - 4
	// but that's adding extra computation on every loop. While this likely isn't
	// very preformance intensive I still think that's a shity sol. Have to bite a
	// bullet. Don't want to harcode into the loop beacasue something like number
	// of blocks should be a constant defined in one place and refrenced everywhere else
	// so changing the value of the variable doesn't require finding every instance of
	// hard coding. Do i really know that I always want n - 4 rows where n is the
	// number of columns? Maybe we scale the game up, then a factor of 4 is what
	// the game needs but I hard coded in - 4

	// initializes all game object when game start, or level resets/changes
	
	int num_boulders = min((this->getLevel() / 2 ) + 2, MAX_BOULDERS);
	// int num_nuggs = max((5 - this->getLevel()) / 2, MAX_GOLD_NUGGETS);
	// int num_oil = min(2 + this->getLevel(), MAX_OIL_BARRELS);

	bool visible; // variable used to assign an Earth block's visibility based off location
	// initialize field of Earth objects
	for (int row = 0; row < ROWS; row++){
		for (int col = 0; col < COLS; col++){
			if (row < 4){ // if bottom 4 rows
				visible = true;
			} else if (row > 59){ // if top 4 rows
				visible = false;
			} else if ((col > 29) && (col < 34)){ // if shaft
				visible = false;
			} else {visible = true;};
			this->field[col][row] = std::move(new Earth(visible, col, row));
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

// i need a function that takes an existing objects location
// and returns whether a proposed location is valid