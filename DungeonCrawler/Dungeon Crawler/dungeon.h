#ifndef DUNGEON_H
#define DUNGEON_H

#include "lib.h"

enum Encounter {
	NONE,
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

std::unordered_map<Difficulty, int> typeMap;

#endif