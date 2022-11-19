#include "StudentWorld.h"
#include "Actor.h"
#include <string>
#include <vector>
#include <random>
#include <algorithm> // this guys needed because find cries without
using namespace std;

// START ALLAN'S CHANGES

// field dimenstions
const int ROWS = 60;
const int COLS = 64;

// max number of spawnanble consumables on a single level
const unsigned int MAX_BOULDERS = 9;
const unsigned int MAX_GOLD_NUGGETS = 2;
const unsigned int MAX_OIL_BARRELS = 21;

// END ALLAN'S CHANGES

// TODO: visiblity of Earth
// Currently clips through shaft because if location is left of shaft,
// boulder spawns but since size is diff than earth, takes up 4x4

GameWorld* createStudentWorld(string assetDir)
{
	return new StudentWorld(assetDir);
}

bool validSpawn(int x1, int y1, int x2, int y2){
	// returns whether or not the distance between point 1
	// and point 2 is > 6. Helper for StudentWorld::init
	float dist = sqrt(pow((x2 - x1), 2) + pow((y2 - y1), 2));
	return dist > 6;
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
	bool distance_cond_met = true;
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
			std::cout << potential_x << endl;
			// changed to 29 - 3. i think this will keep it from clipping the shaft
			if ((potential_x < (29 - 3)) || (potential_x > 34)){ // not at shaft
				// i think this cond ius gargabe. dist produces that so...
				if ((potential_y > 19) && (potential_y < 57)){ // row [20, 56] like spec
					vector<int> temp = {potential_x, potential_y};
					if (find(invalid_locs.begin(), invalid_locs.end(), temp) == invalid_locs.end()){ // location is not taken
						for (auto boulder: this->boulders){ // check distacne against each NE
							if (!(validSpawn(boulder->getX(), boulder->getY(), potential_x, potential_y))){ // if violate distnace rule
								distance_cond_met = false;
								break;
							}
						} if (distance_cond_met){
							this->boulders.push_back(std::move(new Boulder(true, potential_x, potential_y))); // save it
							randomly_placed = true; // break loop
						}
					}
				}
			}

		}
		distance_cond_met = true;
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
			} 
			// i think this condition in no longer necessary because of consts added
			// else if (row > 59){ // if top 4 rows
			// 	visible = false;
			// } 
			// consider not even allocating here
			else if ((col > 29) && (col < 34)){ // if shaft
				visible = false;
			} else if (find(invalid_locs.begin(), invalid_locs.end(), temp) != invalid_locs.end()){ // if a boulder is there
				// there are a couple reason a block should be invisible.
				// in block has same coords as other NE
				// if block is withing coords of ther NE + size.
				// for example a dirt block should not appear in a 4 block swuare of a boulder
				// because the boulder is there, however simply setting any Earth obj's
				// visibility to false if coords match exactly is not enough.
				// if 4 to the right, under or right + under then visibility should be false 
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
