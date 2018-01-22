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

bool DisjointSets::setunion(int a, int b) {
	int parentA, parentB;
	if (set[a] < 0)
		parentA = a;
	else
		parentA = find(a);
	if (set[b] < 0)
		parentB = b;
	else
		parentB = find(b);
	if (parentA == parentB)
		return false;
	int newSize = set[parentA] + set[parentB];
	if (set[parentA] < set[parentB]) {
		set[parentB] = parentA;
		set[parentA] = newSize;
	}
	else {
		set[parentA] = parentB;
		set[parentB] = newSize;
	}
	return true;
}

DisjointSets::DisjointSets() {

}

void DisjointSets::flush() {
	set.resize(0);
}

int DisjointSets::getSize(int i) {
	return -set[find(i)];
}
/* Dungeon class defenitions */
//creates a basic dungeon, once made, apply dungeon quest modifiers to it
Dungeon::Dungeon() {

}
Dungeon::Dungeon(Difficulty type) {
	srand(time(NULL));


	unsigned i;
	int x, y, prev_x, prev_y, down_count;
	int timeout, mult;
	Tile emptyTile(NONE);
	Tile newTile(PATH);
	Direction dir;
	int dead_count;
	dif = type;

	while (1) {
		switch (type) {
		case EASY:
			//9x9 to 10x10
			width = 9 + rand() % 2;
			height = 9 + rand() % 2;
			dead_count = 2;
			mult = 10;
			break;
		case MEDIUM:
			//11x10 to 12x11
			width = 11 + rand() % 2;
			height = 10 + rand() % 2;
			dead_count = 3;
			mult = 5;
			break;
		case HARD:
			//13x11 to 14x12
			width = 13 + rand() % 2;
			height = 11 + rand() % 2;
			dead_count = 4;
			mult = 2;
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
			emptyTile.setPos(i % width, i / width);
			setTile(i, emptyTile);
		}
		x = start_x;
		y = start_y;
		setTile(x + y * width, newTile);
		path_length = 1;
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
						break;
					}
					continue;
				case RIGHT:
					x++;
					break;
				default:
					while(1);
				}
				mapSet.setunion(prev_y*width + prev_x, y*width + x);
				newTile.setPrev(&dungMap[prev_y*width + prev_x]);
				newTile.setPos(x, y);
				setTile(y*width + x, newTile);
				path_length++;
			}
			//literally duct tape
			timeout++;
			if (timeout > 300)
				break;
		}
		if (timeout > 300)
			continue;
		end_x = x;
		end_y = y;
		//require a longish path
		if (path_length > 2*width && path_length < 3*width)
			break;
	}
	//generate dead ends
	deadendProspectGenerate();
	populateDungeon(type);

	//initialize sight in dungeon
	sight.resize(width*height);
	for (i = 0; i < width*height; i++) {
		sight[i].seen = false;
		sight[i].scouted = false;
		sight[i].visited = false;
	}
	
}

void Dungeon::populateDungeon(Difficulty diff) {
	//main path length is between 2*height and 3*height
	//mob encounters for main path
	//dead ends are completely random
	int enc, total_enc;
	int i, j;
	switch (diff) {
	case EASY:
		total_enc = 2 + rand() % 2;
		break;
	case MED:
		total_enc = 3 + rand() % 2;
		break;
	case HARD:
		total_enc = 4 + rand() % 2;
		break;
	}
	enc = total_enc;
	srand(time(NULL));
	int range = path_length/enc;
	int count = rand() % 3;
	Tile* trav = &dungMap[end_x + end_y * width];
	for (i = 0; i < count; i++) {
		trav = trav->getPrev();
	}
	//set it as boss
	trav->setType(BOSS);
	boss = trav;
	//do other stuff to store boss info and such
	boss = trav;

	for (i = 0; i < 3-count; i++) {
		trav = trav->getPrev();
	}

	while (range * enc > path_length-5) {
		range--;
	}

	//set up the ranges for population
	std::vector<std::vector<Tile*>> range_maps;
	range_maps.resize(enc);
	for (i = 0; i < enc; i++) {
		range_maps[i].resize(range);
		for (j = 0; j < range; j++) {
			range_maps[i][j] = trav;
			trav = trav->getPrev();
		}
	}

	srand(time(NULL));
	//set up mob encounters
	int range_num = 0;
	while (enc > 0) {
		count = rand() % range;
		range_maps[range_num][count]->setType(MOB);
		//otherstuff
		mob.push_back(range_maps[range_num][count]);
		enc--;
		range_num++;
	}

	srand(time(NULL));
	//loot encounters
	range_num = 0;
	enc = 1 + rand() % 2;
	while (enc > 0) {
		while (range_maps[range_num][count]->getType() != PATH) {
			count = rand() % range;
		}
		range_maps[range_num][count]->setType(LOOT);
		loot.push_back(range_maps[range_num][count]);
		//otherstuff

		enc--;
		range_num++;
	}

	//info encounters
	srand(time(NULL));
	range_num = 0;
	enc = 2 + rand() % (total_enc - 1);
	while (enc > 0) {
		while (range_maps[range_num][count]->getType() != PATH) {
			count = rand() % range;
		}
		range_maps[range_num][count]->setType(INFO);
		info.push_back(range_maps[range_num][count]);
		//otherstuff

		enc--;
		range_num++;
	}
	int timeout = 0;
	//choice encounters
	srand(time(NULL));
	range_num = 0;
	enc = 2 + rand() % (total_enc - 1);
	while (enc > 0) {
		while (range_maps[range_num][count]->getType() != PATH) {
			count = rand() % range;
			timeout++;
			if (timeout > 300)
				break;
		}
		if (timeout > 300)
			break;
		range_maps[range_num][count]->setType(CHOICE);
		choice.push_back(range_maps[range_num][count]);
		//otherstuff

		enc--;
		range_num++;
	}
	timeout = 0;
	//trap encounters
	srand(time(NULL));
	range_num = 0;
	enc = 2 + rand() % (total_enc - 1);
	while (enc > 0) {
		while (range_maps[range_num][count]->getType() != PATH) {
			count = rand() % range;
			timeout++;
			if (timeout > 300)
				break;
		}
		if (timeout > 300)
			break;
		range_maps[range_num][count]->setIndex(rand() % NUM_TRAP);
		range_maps[range_num][count]->setType(TRAP);
		trap.push_back(range_maps[range_num][count]);
		//otherstuff

		enc--;
		range_num++;
	}

	//fill out deadends with encounters
	srand(time(NULL));
	std::vector<std::vector<Tile*>> dead_range_maps;
	dead_range_maps.resize(deadends.size());
	for (i = 0; i < deadends.size(); i++) {
		dead_range_maps[i].resize(deadends[i].length);
		trav = deadends[i].end;
		for (j = 0; j < deadends[i].length; j++) {
			dead_range_maps[i][j] = trav;
			trav = trav->getPrev();
		}
	}

	for (i = 0; i < deadends.size(); i++) {
		count = rand() % deadends[i].length;

		dead_range_maps[i][count]->setType(CHOICE);
	}
}

void Dungeon::deadendProspectGenerate() {
	//initializations
	int i, j;
	//init the boss area
	initArea();
	//create barriers
	updateBarriers();
	deadends.reserve(6);

	while (1) {
		for (i = 0; i < width; i++) {
			for (j = 1; j < height - 1; j++) {
				if (dungMap[i + j * width].getType() == NONE) {
					//setunion all adjacent tiles, except barriers
					setUnionNone(i, j);
				}
			}
		}
		std::vector<int> possible;
		int highest = 0;
		int curr = 0;
		for (i = 0; i < width; i++) {
			for (j = 0; j < height; j++) {
				//searh for highest prospect
				curr = 0;
				if (dungMap[i + j * width].getType() == BARRIER) {
					//check adjacent tiles for highest prospect
					if (j > 0) {
						if (dungMap[i + (j - 1)*width].getType() == NONE) {
							if (curr < mapSet.getSize(i + (j - 1)*width)) {
								curr = mapSet.getSize(i + (j - 1)*width);
							}
						}
					}
					if (i > 0) {
						if (dungMap[i - 1 + (j)*width].getType() == NONE) {
							if (curr < mapSet.getSize(i - 1 + (j)*width)) {
								curr = mapSet.getSize(i - 1 + (j)*width);
							}
						}

					}
					if (j < height - 1) {
						if (dungMap[i + (j + 1)*width].getType() == NONE) {
							if (curr < mapSet.getSize(i + (j + 1)*width)) {
								curr = mapSet.getSize(i + (j + 1)*width);
							}
						}

					}
					if (i < width - 1) {
						if (dungMap[i + 1 + (j)*width].getType() == NONE) {
							if (curr < mapSet.getSize(i + 1 + (j)*width)) {
								curr = mapSet.getSize(i + 1 + (j)*width);
							}
						}

					}
				}
				if (curr == highest) {
					//push to tile vector
					if(!deadendAdjacent(i, j))
						possible.push_back(i + j * width);
				}
				else if (curr > highest) {
					//flush vector and start a new one
					if(!deadendAdjacent(i, j)) {
						highest = curr;
						possible = std::vector<int>(0);
						possible.push_back(i + j * width);
					}
				}
			}
		}
		if (highest < height)
			break;
		//at this point we have a vector of RMO indeces to barrier tiles with the highest prospect
		srand(time(NULL));
		int selected = possible[rand() % possible.size()];
		dungMap[selected].setType(DEADEND);
		//now generate the path for this deadend
		int d_count = 2;//limits the downs to only 2 which prevents obviously bad paths


		Tile* trav = &dungMap[selected];
		int prev_y, prev_x, y, x, timeout = 0;
		Direction dir;
		Tile newTile(DEADEND);

		prev_x = trav->getX();
		prev_y = trav->getY();
		x = trav->getX();
		y = trav->getY();
		//set all barrier tiles back to NONE
		clearBarriers();
		//grab a fresh disjoint set and only union paths and detours
		reSet();
		int path_limit = (height / 3) + rand() % 3;
		srand(time(NULL));
		int count = 0;
		while (1) {
			while (1) {
				dir = (Direction)(rand() % 4);
				timeout++;
				if (dir == DOWN && d_count < 1)
					continue;
				if (y == 1 && dir == UP)
					continue;
				if (canTravel(x, y, dir))
					break;
				if (timeout > 1000)
					break;
			}
			if (timeout > 1000) {
				timeout = 0;
				break;
			}
			//by this point, a valid direction is selected

			if (canTravel(x, y, dir)) {
				prev_x = x;
				prev_y = y;
				switch (dir) {
				case UP:
					if (canGen(x, y - 1)) {
						y--;
						break;
					}
					continue;
				case LEFT:
					if (canGen(x - 1, y)) {
						x--;
						break;
					}
					continue;
				case DOWN:
					if (canGen(x, y + 1)) {
						y++;
						d_count--;
						break;
					}
					continue;
				case RIGHT:
					if (canGen(x + 1, y)) {
						x++;
						break;
					}
					continue;
				default:
					while (1);
				}
				path_limit--;
				timeout = 0;
				mapSet.setunion(prev_y*width + prev_x, y*width + x);
				newTile.setPrev(&dungMap[prev_y*width + prev_x]);
				newTile.setPos(x, y);
				setTile(y*width + x, newTile);
				count++;
			}
			if (path_limit < 1)
				break;
		}
		Deadend newdead;
		newdead.end = &dungMap[x + y * width];
		newdead.length = count+1;
		deadends.push_back(newdead);
		updateBarriers();
	}
	//after generating deadends, these are useless
	clearBarriers();
	deadends.shrink_to_fit();
}

void Dungeon::reSet() {
	//new set
	mapSet = DisjointSets();
	mapSet.addelements(width*height);
	//union all non NONE tiles
	int i, j;
	for (i = 0; i < width; i++) {
		for (j = 1; j < height - 1; j++) {
			if (dungMap[i + j * width].getType() != NONE)
				mapSet.setunion(start_x + start_y * width, i + j * width);
		}
	}
}

void Dungeon::clearBarriers() {
	int i, j;
	for (i = 0; i < width; i++) {
		for (j = 1; j < height - 1; j++) {
			if (dungMap[i + j * width].getType() == BARRIER)
				dungMap[i + j * width].setType(NONE);
		}
	}
}

void Dungeon::updateBarriers() {
	int i, j;
	for (i = 0; i < width; i++) {
		for (j = 1; j < height - 1; j++) {
			if (canGen(i, j) && dungMap[i + j * width].getType() == NONE && !inBossArea(i, j))
				dungMap[i + j * width].setType(BARRIER);
		}
	}
}

void Dungeon::initArea() {
	//the boss area is the last 3 tiles and adjacent areas, store all tiles to vec to be compared later

	//create boss area
	int x = end_x;
	int y = end_y;
	int temp_x;
	for (int i = 0; i < 3; i++) {
		//add all adjacent tiles
		if (y > 0) {
			if (getTile(x + (y - 1)*width)->getType() == BARRIER || getTile(x + (y - 1)*width)->getType() == NONE)
				dungMap[x + (y - 1)*width].setArea(BOSS_);
		}
		if (x > 0) {
			if (getTile(x - 1 + (y)*width)->getType() == BARRIER || getTile(x - 1 + (y)*width)->getType() == NONE)
				dungMap[x - 1 + (y)*width].setArea(BOSS_);
		}
		if (y < getHeight() - 1) {
			if (getTile(x + (y + 1)*width)->getType() == BARRIER || getTile(x + (y + 1)*width)->getType() == NONE)
				dungMap[x + (y + 1)*width].setArea(BOSS_);
		}
		if (x < getWidth() - 1) {
			if (getTile(x + 1 + (y)*width)->getType() == BARRIER || getTile(x + 1 + (y)*width)->getType() == NONE)
				dungMap[x + 1 + (y)*width].setArea(BOSS_);
		}
		temp_x = x;
		x = dungMap[temp_x + y * width].getPrev()->getX();
		y = dungMap[temp_x + y * width].getPrev()->getY();
	}


}

bool Dungeon::inBossArea(int x, int y) {
	if (dungMap[x + y * width].getArea() == BOSS_ || dungMap[x + y * width].getArea() == START_)
		return true;
	return false;
}

bool Dungeon::canGen(int x, int y) {
	bool path = false;
	if (y > 0) {
		if (dungMap[x + (y - 1)*width].getType() == PATH || dungMap[x + (y - 1)*width].getType() == DEADEND)
			path = true;
	}
	if (x > 0) {
		if (dungMap[x - 1 + (y)*width].getType() == PATH || dungMap[x - 1 + (y)*width].getType() == DEADEND) {
			if (path == true)
				return false;
			path = true;
		}
	}
	if (y < height - 1) {
		if (dungMap[x + (y + 1)*width].getType() == PATH || dungMap[x + (y + 1)*width].getType() == DEADEND) {
			if (path == true)
				return false;
			path = true;
		}
	}
	if (x < width - 1) {
		if (dungMap[x + 1 + (y)*width].getType() == PATH || dungMap[x + 1 + (y)*width].getType() == DEADEND) {
			if (path == true)
				return false;
			path = true;
		}
	}
	return path;
}

bool Dungeon::pathAdjacent(int x, int y) {
	bool path = false;
	if (y > 0) {
		if (dungMap[x + (y - 1)*width].getType() == PATH || dungMap[x + (y - 1)*width].getType() == DEADEND)
			path = true;
	}
	if (x > 0) {
		if (dungMap[x - 1 + (y)*width].getType() == PATH || dungMap[x - 1 + (y)*width].getType() == DEADEND) {
			path = true;
		}
	}
	if (y < height - 1) {
		if (dungMap[x + (y + 1)*width].getType() == PATH || dungMap[x + (y + 1)*width].getType() == DEADEND) {
			path = true;
		}
	}
	if (x < width - 1) {
		if (dungMap[x + 1 + (y)*width].getType() == PATH || dungMap[x + 1 + (y)*width].getType() == DEADEND) {
			path = true;
		}
	}
	return path;
}

bool Dungeon::deadendAdjacent(int x, int y) {
	bool path = false;
	if (y > 0) {
		if (dungMap[x + (y - 1)*width].getType() == DEADEND)
			path = true;
	}
	if (x > 0) {
		if (dungMap[x - 1 + (y)*width].getType() == DEADEND) {
			path = true;
		}
	}
	if (y < height - 1) {
		if (dungMap[x + (y + 1)*width].getType() == DEADEND) {
			path = true;
		}
	}
	if (x < width - 1) {
		if (dungMap[x + 1 + (y)*width].getType() == DEADEND) {
			path = true;
		}
	}
	return path;
}

void Dungeon::setUnionNone(int x, int y) {
	//union everythign adjacent as long as its NONE
	int curr = x + (y)*width;
	if (y > 1) {
		if (getTile(x + (y - 1)*width)->getType() == NONE && !pathAdjacent(x, y-1) && mapSet.find(curr) != mapSet.find(curr - width))
			mapSet.setunion(curr, curr - width);
	}
	if (x > 0) {
		if (getTile(x - 1 + (y)*width)->getType() == NONE && !pathAdjacent(x - 1, y) && mapSet.find(curr) != mapSet.find(curr - 1))
			mapSet.setunion(curr, curr - 1);
	}
	if (y < getHeight() - 2) {
		if (getTile(x + (y + 1)*width)->getType() == NONE && !pathAdjacent(x, y + 1) && mapSet.find(curr) != mapSet.find(curr + width))
			mapSet.setunion(curr, curr + width);
	}
	if (x < getWidth() - 1) {
		if (getTile(x + 1 + (y)*width)->getType() == NONE && !pathAdjacent(x + 1, y) && mapSet.find(curr) != mapSet.find(curr + 1))
			mapSet.setunion(curr, curr + 1);
	}
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
	if (mapSet.find(x + y * width) == mapSet.find(curr_x + curr_y * width))
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

void Dungeon::updateLOS() {
	//current tile as visited
	switch (gParty.getLOS()) {
	case 2:
		//extra looking
		if (gParty.getY() > 1 ) {
			if(dungMap[gParty.getX() + (gParty.getY()-1)*width].getType() != NONE)
				sight[gParty.getX() + (gParty.getY() - 2)*width].seen = true;
		}
		if (gParty.getX() > 1) {
			if (dungMap[gParty.getX() - 1 + (gParty.getY())*width].getType() != NONE)
				sight[gParty.getX() - 2 + gParty.getY()*width].seen = true;
		}
		if (gParty.getY() < height - 2) {
			if (dungMap[gParty.getX() + (gParty.getY() + 1)*width].getType() != NONE)
				sight[gParty.getX() + (gParty.getY() + 2)*width].seen = true;
		}
		if (gParty.getX() < width - 2) {
			if (dungMap[gParty.getX() + 1 + (gParty.getY())*width].getType() != NONE)
				sight[gParty.getX() + 2 + gParty.getY()*width].seen = true;
		}

		if (gParty.getY() > 0 && gParty.getX() > 0) {
			if (dungMap[gParty.getX() + (gParty.getY() - 1)*width].getType() != NONE || dungMap[gParty.getX() - 1 + (gParty.getY())*width].getType() != NONE)
				sight[gParty.getX() - 1 + (gParty.getY() - 1)*width].seen = true;
		}
		if (gParty.getX() > 0 && gParty.getY() < height - 1) {
			if (dungMap[gParty.getX() + (gParty.getY() + 1)*width].getType() != NONE || dungMap[gParty.getX() - 1 + (gParty.getY())*width].getType() != NONE)
				sight[gParty.getX() - 1 + (gParty.getY()+1)*width].seen = true;
		}
		if (gParty.getX() < width - 1 && gParty.getY() < height - 1) {
			if (dungMap[gParty.getX() + (gParty.getY() + 1)*width].getType() != NONE || dungMap[gParty.getX() + 1 + (gParty.getY())*width].getType() != NONE)
				sight[gParty.getX() + 1 + (gParty.getY() + 1)*width].seen = true;
		}
		if (gParty.getX() < width - 1 && gParty.getY() > 0) {
			if (dungMap[gParty.getX() + (gParty.getY() - 1)*width].getType() != NONE || dungMap[gParty.getX() + 1 + (gParty.getY())*width].getType() != NONE)
				sight[gParty.getX() + 1 + (gParty.getY()-1)*width].seen = true;
		}
	case 1:
		sight[gParty.getX() + gParty.getY()*width].visited = true;
		if (gParty.getY() > 0) {
			sight[gParty.getX() + (gParty.getY() - 1)*width].seen = true;
		}
		if (gParty.getX() > 0) {
			sight[gParty.getX() - 1 + gParty.getY()*width].seen = true;
		}
		if (gParty.getY() < height - 1) {
			sight[gParty.getX() + (gParty.getY() + 1)*width].seen = true;
		}
		if (gParty.getX() < width - 1) {
			sight[gParty.getX() + 1 + gParty.getY()*width].seen = true;
		}
		break;
	}
	

}

bool Dungeon::perceptionCheck() {
	int highest_wis = (gParty.getChar(0)->getWis() < gParty.getChar(1)->getWis()) ? gParty.getChar(1)->getWis() : gParty.getChar(0)->getWis();
	highest_wis = (highest_wis < gParty.getChar(2)->getWis()) ? gParty.getChar(2)->getWis() : highest_wis;
	srand(time(NULL));
	int roll = rand() % 20 + 1;
	//automatic fail
	if (roll == 1)
		return false;
	int x = gParty.getX();
	int y = gParty.getY();
	//what need to be beat
	int DC;
	switch (dif) {
	case EASY:
		DC = 0;
		break;
	case MED:
		DC = 1;
		break;
	case HARD:
		DC = 3;
		break;
	}
	switch (dungMap[x + (y)*width].getType()) {
	case MOB:
		DC += 12;
		break;
	case BOSS:
		DC += 12;
		break;
	case TRAP:
		DC += 12;
		break;
	case LOOT:
		DC += 10;
		break;
	case INFO:
		DC += 10;
		break;
	case CHOICE:
		DC += 10;
		break;
	default:
		return false;
	}
	if (sight[x + (y)*width].scouted)
		roll += 5;
	return (roll + highest_wis > DC);
	/*if (y > 0 ) {
		//if the tile is just a path, not an empty tile, and has never been seen before, attempt to see
		if (dungMap[x + (y - 1)*width].getType() != PATH && dungMap[x + (y - 1)*width].getType() != NONE && !getSeen(x + (y - 1)*width) && dungMap[x + (y - 1)*width].getType() != DEADEND) {
			switch (dungMap[x + (y - 1)*width].getType()) {
			case MOB:
				msg = "The party notices some enemies ";
				DC += 12;
				break;
			case BOSS:
				msg = "The party sees the boss ";
				DC += 12;
				break;
			case TRAP:
				msg = "The party notices a trap ";
				DC += 12;
				break;
			case LOOT:
				msg = "The party sees something shiny ";
				DC += 10;
				break;
			case INFO:
				msg = "The party sees some ancient text ";
				DC += 10;
				break;
			case CHOICE:
				msg = "The party sees a well ";
				DC += 10;
				break;
			}
			if (roll + highest_wis > DC) {
				sight[x + (y - 1)*width].scouted = true;
				//push a message into the message queue
				msg_queue.push(msg + "up above.");
			}
		}
	}
	DC = stash;
	if (gParty.getX() > 0) {
		if (dungMap[x - 1 + (y)*width].getType() != PATH && dungMap[x - 1 + (y)*width].getType() != NONE && !getSeen(x - 1 + (y)*width) && dungMap[x - 1 + (y)*width].getType() != DEADEND) {
			switch (dungMap[x - 1 + (y)*width].getType()) {
			case MOB:
				msg = "The party notices some enemies ";
				DC += 12;
				break;
			case BOSS:
				msg = "The party sees the boss ";
				DC += 12;
				break;
			case TRAP:
				msg = "The party notices a trap ";
				DC += 12;
				break;
			case LOOT:
				msg = "The party sees something shiny ";
				DC += 10;
				break;
			case INFO:
				msg = "The party sees some ancient text ";
				DC += 10;
				break;
			case CHOICE:
				msg = "The party sees a well ";
				DC += 10;
				break;
			}
			if (roll + highest_wis > DC) {
				sight[x - 1 + (y)*width].scouted = true;
				//push a message into the message queue
				msg_queue.push(msg + "to the left.");

			}
		}
	}
	DC = stash;
	if (gParty.getY() < height - 1) {
		if (dungMap[x + (y + 1)*width].getType() != PATH && dungMap[x + (y + 1)*width].getType() != NONE && !getSeen(x + (y + 1)*width) && dungMap[x + (y + 1)*width].getType() != DEADEND) {
			switch (dungMap[x + (y + 1)*width].getType()) {
			case MOB:
				msg = "The party notices some enemies ";
				DC += 12;
				break;
			case BOSS:
				msg = "The party sees the boss ";
				DC += 12;
				break;
			case TRAP:
				msg = "The party notices a trap ";
				DC += 12;
				break;
			case LOOT:
				msg = "The party sees something shiny ";
				DC += 10;
				break;
			case INFO:
				msg = "The party sees some ancient text ";
				DC += 10;
				break;
			case CHOICE:
				msg = "The party sees a well ";
				DC += 10;
				break;
			}
			if (roll + highest_wis > DC) {
				sight[x + (y + 1)*width].scouted = true;
				//push a message into the message queue
				msg_queue.push(msg + "down below.");

			}
		}
	}
	DC = stash;
	if (gParty.getX() < width - 1) {
		if (dungMap[x + 1 + (y)*width].getType() != PATH && dungMap[x + 1 + (y)*width].getType() != NONE && !getSeen(x + 1 + (y)*width) && dungMap[x + 1 + (y)*width].getType() != DEADEND) {
			switch (dungMap[x + 1 + (y)*width].getType()) {
			case MOB:
				msg = "The party notices some enemies ";
				DC += 12;
				break;
			case BOSS:
				msg = "The party sees the boss ";
				DC += 12;
				break;
			case TRAP:
				msg = "The party notices a trap ";
				DC += 12;
				break;
			case LOOT:
				msg = "The party sees something shiny ";
				DC += 10;
				break;
			case INFO:
				msg = "The party sees some ancient text ";
				DC += 10;
				break;
			case CHOICE:
				msg = "The party sees a well ";
				DC += 10;
				break;
			}
			if (roll + highest_wis > DC) {
				sight[x + 1 + (y)*width].scouted = true;
				//push a message into the message queue
				msg_queue.push(msg + "to the right.");
			}
		}
	}*/
}

void Dungeon::setTile(int RMO_index, Tile newTile) {
	dungMap[RMO_index] = newTile;
}
Tile* Dungeon::getTile(int RMO_index) {
	return &dungMap[RMO_index];
}
int Dungeon::getWidth() { return width; }
int Dungeon::getHeight() { return height; }
bool Dungeon::isStart(int x, int y) { return ((x == start_x && y == start_y) ? true : false); }
bool Dungeon::isEnd(int x, int y) { return ((x == end_x && y == end_y) ? true : false); }
bool Dungeon::isEncounter(int x, int y) { return ((dungMap[x + y * width].getType() != PATH && dungMap[x + y * width].getType() != DEADEND) ? true : false); }
int Dungeon::getStartX() { return start_x; }
int Dungeon::getStartY() { return start_y; }
Tile* Dungeon::getBoss() { return boss; }
std::vector<Tile*> *Dungeon::getLoot() { return &loot; }
std::vector<Tile*> *Dungeon::getMob() { return &mob; }
std::vector<Tile*> *Dungeon::getInfo() { return &info; }
bool Dungeon::getSightStatus(int i) { return (sight[i].scouted || sight[i].visited || sight[i].seen); }
bool Dungeon::getSeen(int i) { return sight[i].seen; }
bool Dungeon::getVisited(int i) { return sight[i].visited; }
bool Dungeon::getScouted(int i) { return sight[i].scouted; }
void Dungeon::scoutTile(int i) {
	//i is RMO index
	sight[i].scouted = true;
	EncounterType no = dungMap[i].getType();
	if (no == DEADEND || no == PATH)
		return;
	srand(time(NULL));
	EncounterType alt = no;
	while(alt == no || alt == BOSS || alt == DEADEND || alt == PATH || alt == BARRIER || alt == NONE)
		alt = (EncounterType)(rand() % (sizeof(EncounterType)));
	dungMap[i].setAlt(alt);
}


/* Tile class defenitions */
//Tile defines each individual (x,y) location in the map
//contains info for whatever the tile represents
Tile::Tile(EncounterType type_) {
	type = type_;
	prev = NULL;
	x = -1;
	y = -1;
	area = NONE_;
	index = -1;
}

Tile::Tile() {
	type = NONE;
	prev = NULL;
	x = -1;
	y = -1;
	area = NONE_;
	index = -1;
}

Tile* Tile::getPrev() { return prev; }
void Tile::setPrev(Tile* prev_) {prev = prev_;}

int Tile::getX() { return x; }
int Tile::getY() { return y; }
void Tile::setPos(int x_, int y_) {x = x_;y = y_;}

EncounterType Tile::getType() {return type;}
EncounterType Tile::getAlt() { return alt; }
void Tile::setType(EncounterType type_) { type = type_; }
int Tile::getIndex() { return index; }
void Tile::setIndex(int i) { index = i; }
void Tile::setAlt(EncounterType alt_) { alt = alt_; }

void Tile::operator=(const Tile& t) {
	type = t.type;
	prev = t.prev;
	x = t.x;
	y = t.y;
}

void Tile::setArea(Area a) { area = a; }
Area Tile::getArea() { return area; }

/* misc functions */
std::string diffToString(Difficulty diff) {
	switch (diff) {
	case EASY:
		return "Easy";
	case MEDIUM:
		return "Medium";
	case HARD:
		return "Hard";
	default:
		return "WUTT";
	}
}
