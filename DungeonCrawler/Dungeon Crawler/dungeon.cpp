#include "dungeon.h"
#include "SDL.h"

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

Dungeon::Dungeon(Difficulty type) {
	srand(NULL);
	int width, height;
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
	}
	int start_index = rand() % (width + height/2);
	int start_y, start_x;
	if (start_index < height / 2) {
		start_y = height / 2 + start_index;
		start_x = 0;
	}
	else {
		start_y = 0;
		start_x = start_index - height / 2;
	}
	unsigned i;
	for (i = 0; i < width*height; i++) {
		dungMap.push_back(NONE);
	}
	int x = start_x;
	int y = start_y;

	Direction dir;
	dungMap[start_y*width + start_x] = PATH;
	while (y != 0) {
		dir = (Direction)(rand() % 3);
		switch (dir) {
		case LEFT:
			if (x != 0)
				x--;
			break;
		case UP:
			y++;
			break;
		case RIGHT:
			if (x < width-1)
				x++;
			break;
		}
		dungMap[y*width + x] = PATH;
		//set prev ptr, also ceck dis sets to not make cycles
	}
}






