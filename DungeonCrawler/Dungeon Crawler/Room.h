#ifndef ROOM_H
#define ROOM_H

#include "lib.h"
#include "party.h"



struct RoomTile {
	bool up;
	bool left;
	bool down;
	bool right;
	UnitType type;
	Highlight color;
	//used during move to determine what is in range
	//also used for abilities to determine what is in range
	bool special;
};

class Room
{
public:
	Room();
	void rollInit(std::string mobname);
	void placeUnits();
	void passControl();
	void clearRange();
	int getWidth();
	int getHeight();
	RoomTile* getTile(int x, int y);
	Unit* getCurrUnit();
	int getInitIndex();
	void setInitIndex(int i) { init_index = i; }
	std::vector<Unit*>* getInititiveOrder();
	void checkState();
	void killEnemy() { enemy_count--; }
	void killCharacter() { char_count--; }
	bool notWallAdj(int index);
private:
	//maintain for initiative purposes
	std::vector<Unit*> unitList;

	std::vector<RoomTile> roomMap;
	int width, height;
	int init_index;
	int char_count;
	int enemy_count;
};


extern Room* room;

#endif

