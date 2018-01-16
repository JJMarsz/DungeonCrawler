#ifndef DUNGEON_H
#define DUNGEON_H

#include "lib.h"
#include "LButton.h"

enum EncounterType {
	NONE,
	PATH,
	ITEM,
	INFO,
	BOSS,
	MOBS,
	DEADEND,
	BARRIER,
	AREA1,
	AREA2,
	AREA3,
	AREA4,
	AREA5
};

enum Difficulty {
	EASY,
	MEDIUM,
	HARD
};

enum Direction {
	LEFT,
	UP,
	RIGHT,
	DOWN
};

enum Area {
	BOSS_,
	START_,
	NONE_
};

class Tile {
public:
	Tile(EncounterType type_);
	Tile();
	void operator=(const Tile& t);
	void setPrev(Tile* prev_);
	Tile* getPrev();
	EncounterType getType();
	void setType(EncounterType type_);
	int getX();
	int getY();
	void setPos(int x_, int y_);
	void setArea(Area a);
	Area getArea();
private:
	EncounterType type;
	Tile* prev;
	int x, y;
	Area area;
	//moblist
};

class DisjointSets {
public:
	DisjointSets();
	void flush();
	void addelements(int num);
	int find(int elem);
	bool setunion(int a, int b);
	int getSize(int i);

private:
	std::vector<int> set;
};

class Dungeon {
public:
	Dungeon();
	Dungeon(Difficulty type);
	void setTile(int RMO_index, Tile newTile);
	Tile getTile(int RMO_index);
	int getWidth();
	int getHeight();
	bool isStart(int x, int y);
	bool isEnd(int x, int y);
	int getStartX();
	int getStartY();

private:

	bool pathAdjacent(int x, int y);
	bool deadendAdjacent(int x, int y);
	bool canGen(int x, int y);
	void deadendProspectGenerate();
	void setUnionNone(int x, int y);
	void initArea();
	bool inBossArea(int x, int y);
	bool canTravel(int x, int y, Direction dir);
	void clearBarriers();
	void updateBarriers();
	void reSet();

	//use RMO to access
	//x + width*y
	std::vector<Tile> dungMap;
	DisjointSets mapSet;
	std::unordered_map<int, bool> bossArea;
	int width, height;
	int start_x, start_y;
	int end_x, end_y;
	int path_length;
};

std::string diffToString(Difficulty diff);

extern Dungeon current_dungeon;
#endif