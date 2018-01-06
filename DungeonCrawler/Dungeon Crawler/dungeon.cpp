#include "dungeon.h"
#include "SDL.h"

Dungeon current_dungeon;

/* Disjoint Sets class definitions */
void DisjointSets::addelements(int num) {
	for (int i = 0; i<num; i++) {
		set.push_back(-1);
	}
}

int DisjointSets::find(int elem) {
	if (set[elem] < 0)
		return elem;
	else {
		set[elem] = find(set[elem]);
		return set[elem];
	}
}

void DisjointSets::setunion(int a, int b) {
	int parentA, parentB;
	if (set[a] < 0)
		parentA = a;
	else
		parentA = find(a);
	if (set[b] < 0)
		parentB = b;
	else
		parentB = find(b);
	int newSize = set[parentA] + set[parentB];
	if (set[parentA] < set[parentB]) {
		set[parentB] = parentA;
		set[parentA] = newSize;
	}
	else {
		set[parentA] = parentB;
		set[parentB] = newSize;
	}
}

DisjointSets::DisjointSets() {

}

void DisjointSets::flush() {
	set.resize(0);
}
/* Dungeon class defenitions */
//creates a basic dungeon, once made, apply dungeon quest modifiers to it
Dungeon::Dungeon() {

}
Dungeon::Dungeon(Difficulty type) {
	srand(time(NULL));

	unsigned i;
	int x, y, prev_x, prev_y, down_count;
	int timeout;
	Tile emptyTile(NONE);
	Tile newTile(PATH);
	Direction dir;
	while (1) {
		switch (type) {
		case EASY:
			//4x4 to 6x6
			width = 4 + rand() % 3;
			height = 4 + rand() % 3;
			break;
		case MEDIUM:
			//7x7 to 9x9
			width = 7 + rand() % 3;
			height = 7 + rand() % 3;
			break;
		case HARD:
			//10x10 to 12x12
			width = 10 + rand() % 3;
			height = 10 + rand() % 3;
			break;
		default:
			//problemmmmm
			break;
		}
		start_x = rand() % width;
		start_y = height - 1;;
		//initiliaze the map
		dungMap.resize(width*height);
		mapSet.flush();
		mapSet.addelements(width*height);

		for (i = 0; (int)i < width*height; i++) {
			setTile(i, emptyTile);
		}
		x = start_x;
		y = start_y;
		setTile(x + y * width, newTile);
		path_length = 0;
		down_count = height / 2;
		timeout = 0;
		while (y != 0) {
			dir = (Direction)(rand() % 4);
			if (canTravel(x, y, dir)) {
				prev_x = x;
				prev_y = y;
				switch (dir) {
				case UP:
					y--;
					break;
				case LEFT:
					x--;
					break;
				case DOWN:
					if (down_count > 0) {
						y++;
						down_count--;
					}
					break;
				case RIGHT:
					x++;
					break;
				default:
					continue;
				}
				newTile.setPrev(&dungMap[prev_y*width + prev_x]);
				setTile(y*width + x, newTile);
				mapSet.setunion(prev_y*width + prev_x, y*width + x);
				path_length++;
			}
			timeout++;
			if (timeout > 300)
				break;
		}
		if (timeout > 300)
			continue;
		end_x = x;
		end_y = y;
		if (path_length > 2*height - height/2)
			break;
	}
	
	//generate dead ends
}

bool Dungeon::canTravel(int curr_x, int curr_y, Direction dir) {
	//boundary checks
	if (curr_x < 0 || curr_x >= width || curr_y < 0 || curr_y >= height)
		return false;
	//check if a cycle is made
	//all adjacent 4 tiles need to be cleard
	int x, y;
	switch (dir) {
	case UP:
		x = curr_x;
		y = curr_y - 1;
		break;
	case LEFT:
		x = curr_x - 1;
		y = curr_y;
		break;
	case DOWN:
		x = curr_x;
		y = curr_y + 1;
		break;
	case RIGHT:
		x = curr_x + 1;
		y = curr_y;
		break;
	}
	if (x < 0 || x >= width || y < 0 || y >= height)
		return false;
	std::unordered_map<int, bool> comp;
	if (y < height - 1)
		comp[mapSet.find(x + (y + 1)*width)] = true;
	if (y > 0) {
		if (comp.find(mapSet.find(x + (y - 1)*width)) != comp.end())
			return false;
		comp[mapSet.find(x + (y - 1)*width)] = true;
	}
	if (x > 0) {
		if (comp.find(mapSet.find(x - 1 + (y)*width)) != comp.end())
			return false;
		comp[mapSet.find(x - 1 + (y)*width)] = true;
	}
	if (x < width - 1) {
		if (comp.find(mapSet.find(x + 1 + (y)*width)) != comp.end())
			return false;
		comp[mapSet.find(x + 1 + (y)*width)] = true;
	}
	//x and y are within the map and do not create a cycle
	return true;

}

void Dungeon::setTile(int RMO_index, Tile newTile) {
	dungMap[RMO_index] = newTile;
}

Tile Dungeon::getTile(int RMO_index) {
	return dungMap[RMO_index];
}


int Dungeon::getWidth() { return width; }
int Dungeon::getHeight() { return height; }



/* Tile class defenitions */
//Tile defines each individual (x,y) location in the map
//contains info for whatever the tile represents
Tile::Tile(Encounter type_) {
	type = type_;
	prev = NULL;
}

Tile::Tile() {
	type = NONE;
	prev = NULL;
}

void Tile::setPrev(Tile* prev_) {
	prev = prev_;
}


Encounter Tile::getType() {
	return type;
}
