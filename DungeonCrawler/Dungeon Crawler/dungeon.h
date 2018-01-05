#ifndef DUNGEON_H
#define DUNGEON_H

#include "lib.h"
#include "LButton.h"

enum Encounter {
	NONE,
	PATH,
	ITEM,
	INFO,
	BOSS,
	MOBS
};

enum Difficulty {
	EASY,
	MEDIUM,
	HARD
};

enum Direction {
	LEFT,
	UP,
	RIGHT
};

class Tile {
public:

private:
	Encounter type;
	bool access;
	//moblist
};

class DisjointSets {
public:
	//DisjointSets();
	void addelements(int num);
	int find(int elem);
	void setunion(int a, int b);
	//int size(elem);

private:
	std::vector<int> set;
};

class Dungeon {
public:
	Dungeon(Difficulty type);

private:
	void setTile(int x, int y, Tile newTile);
	int width, height;
	//use RMO to access
	//x + width*y
	std::vector<Tile> dungMap;
};

class Tile {
public:
	Tile(Encounter type_);

private:
	Encounter type;
};

#endif