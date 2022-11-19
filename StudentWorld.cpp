#include "GameConstants.h"
#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <random>
#include <algorithm>
using namespace std;

// TODO: Check naive approach and visiblity of Earth

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
	
	// used in naive approach to create valid NE location
	std::random_device rd; // random num from hardware
    std::mt19937 gen(rd()); // seed the generator
    std::uniform_int_distribution<> col_dist(0, 60); // set range, inclusive
	std::uniform_int_distribution<> row_dist(20, 56); // could be improved, only 
	bool randomly_placed = false;
	vector<vector<int>> invalid_locs;
	// place boulders
	int num_boulders = min((this->getLevel() / 2 ) + 2, MAX_BOULDERS);
	// naive approach so I can at least display them + create logic to clear earth
	for (int i = 0; i < num_boulders; i++){ // for num_boulders
		while (!randomly_placed){ // until a valid coord is found
			int potential_x, potential_y; // randomly generate x and y
			potential_x = col_dist(gen);
			potential_y = row_dist(gen);
			if ((potential_x < 29) || (potential_x > 34)){ // not at shaft
				if ((potential_y > 19) && (potential_y < 57)){ // row [20, 56] like spec
					vector<int> temp = {potential_x, potential_y};
					if (find(invalid_locs.begin(), invalid_locs.end(), temp) == invalid_locs.end()){ // location is not taken
						// need to check via naive approach
						randomly_placed = true; // break loop
						this->boulders.push_back(std::move(new Boulder(true, potential_x, potential_y))); // save it
					}
				}
			}

		}
		randomly_placed = false; // reset for next iter
	}

	// int num_nuggs = max((5 - this->getLevel()) / 2, MAX_GOLD_NUGGETS);
	// int num_oil = min(2 + this->getLevel(), MAX_OIL_BARRELS);

	bool visible; // variable used to assign an Earth block's visibility based off location
	// initialize field of Earth objects + clear out earth where boulder exists
	for (int row = 0; row < ROWS; row++){
		for (int col = 0; col < COLS; col++){
			vector<int> temp = {col, row};
			if (row < 4){ // if bottom 4 rows
				visible = true;
			} else if (row > 59){ // if top 4 rows
				visible = false;
			} else if ((col > 29) && (col < 34)){ // if shaft
				visible = false;
			} else if (find(invalid_locs.begin(), invalid_locs.end(), temp) != invalid_locs.end()){ // if a boulder is there
				visible = false; // not working. supposed to handle Earth that exists under Dirt. I think it's becaue 4 blocks?
			}
			else {visible = true;};
			this->field[col][row] = std::move(new Earth(visible, col, row));
		}
	}
	// initialize other NEs
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

	// frees memory from Boulder object
	for (int i = this->boulders.size(); i >= 0; i--){
		delete this->boulders[i];
		this->boulders.erase(this->boulders.begin() + i - 1); // this may break but i think it's fine
	}
}

// i need a function that takes an existing objects location
// and returns whether a proposed location is valid