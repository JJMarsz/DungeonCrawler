#include "Room.h"
#include "Quest.h"
#include "Mob.h"

Room* room = NULL;
std::vector<Mob> enemyList;
//used to sort in descending order
bool wayToSort(int i, int j) { return i > j; }

Room::Room(){
	init_index = 0;
	char_count = 0;
	enemy_count = 0;
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	switch (current_quests[quest_index].getDiff()) {
	case EASY:
		height = 7 + rand() % 2;
		width = 8 + rand() % 2;
		break;
	case MEDIUM:
		height = 9 + rand() % 2;
		width = 10 + rand() % 2;
		break;
	case HARD:
		height = 11 + rand() % 2;
		width = 12 + rand() % 2;
		break;
	}
	roomMap.resize(width*height);
	//init the map
	int i, j;
	for (i = 0; i < width; i++) {
		for (j = 0; j < height; j++) {
			roomMap[i + j*width].special = true;
			roomMap[i + j*width].type = NOTHING;
			if (i > 0)
				roomMap[i + j*width].left = false;
			else
				roomMap[i + j * width].left = true;
			if (j > 0)
				roomMap[i + j * width].up = false;
			else
				roomMap[i + j * width].up = true;
			if (i < width - 1)
				roomMap[i + j * width].right = false;
			else
				roomMap[i + j * width].right = true;
			if (j < height - 1)
				roomMap[i + j * width].down = false;
			else
				roomMap[i + j * width].down = true;
		}
	}
	//generate walls
	int index;
	index = rand() % (width*height);
	for (i = 0; i < height; i++) {
		while (roomMap[index].type == WALL) {
			index = rand() % (width*height);
			if (index%width == 0)
				index++;
			else if (index%width == width - 1)
				index--;
			else if (index / width == 0)
				index += width;
			else if (index / width == height - 1)
				index -= width;
		}
		roomMap[index].type = WALL;
		roomMap[index].left = true;
		roomMap[index].right = true;
		roomMap[index].down = true;
		roomMap[index].up = true;
	}


}

int Room::getWidth() { return width; }
int Room::getHeight() { return height; }
RoomTile* Room::getTile(int x, int y) { return &roomMap[x + y * width]; }
Unit* Room::getCurrUnit() { return unitList[init_index]; }
int Room::getInitIndex() { return init_index; }
std::vector<Unit*>* Room::getInititiveOrder() { return &unitList; }

void Room::rollInit(std::string quest_name) {
	//only dealing with chars for now
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	std::unordered_map<int, Unit*> map;
	std::vector<int> init;
	int i, mod = 0;
	if (current_dungeon.getScouted(gParty->getX() + gParty->getY()*current_dungeon.getWidth()))
		mod = 5;
	for (i = 0; i < 3; i++) {
		if (gParty->getChar(i)->isAlive()) {
			int roll = rand() % 20 + mod + gParty->getChar(i)->getDex();
			while (map.find(roll) != map.end())
				roll--;
			map[roll] = gParty->getChar(i);
			init.push_back(roll);
			char_count++;
		}
	}
	//add mobs
	if (gParty->getX() + gParty->getY()*current_dungeon.getWidth() == current_dungeon.getBoss()->getX() + current_dungeon.getBoss()->getY()*current_dungeon.getWidth()) {
		enemy_count = 1;
		enemyList.resize(enemy_count);
		//generate boss
		Mob newb = mobEncMap[quest_name][mobEncMap[quest_name].size() - 1];
		int roll = rand() % 20 + 1 + newb.getDex();
		while (map.find(roll) != map.end())
			roll++;
		enemyList[0] = mobEncMap[quest_name][mobEncMap[quest_name].size() - 1];
		map[roll] = &enemyList[0];
		init.push_back(roll);
	}
	else{
		switch (current_quests[quest_index].getDiff()) {
		case EASY:
			enemy_count = 3;
			break;
		case MEDIUM:
			enemy_count = 4;
			break;
		case HARD:
			enemy_count = 5;
			break;
		}
		enemyList.resize(enemy_count);
		for (i = 0; i < enemy_count; i++) {
			int index = rand() % (mobEncMap[quest_name].size() - 1);
			Mob newbie = mobEncMap[quest_name][index];
			int roll = rand() % 20 + 1 + newbie.getDex();
			while (map.find(roll) != map.end())
				roll--;
			enemyList[i] = mobEncMap[quest_name][index];
			map[roll] = &enemyList[i];
			init.push_back(roll);
		}
	}
	unitList.resize(MAX_CHAR + enemy_count);

	//sort create sorted list
	std::sort(init.begin(), init.end(), wayToSort);
	for (i = 0; i < init.size(); i++) {
		unitList[i] = map[init[i]];
	}
}

void Room::placeUnits() {
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	//find the direction in which the party came from
	int dung_width = current_dungeon.getWidth();
	Tile* curr = current_dungeon.getTile(gParty->getX() + gParty->getY()*dung_width);
	Tile* prev = curr->getPrev();
	int prev_RMO = prev->getX() + prev->getY()*dung_width;
	int curr_RMO = curr->getX() + curr->getY()*dung_width;
	Direction dir;
	if (curr_RMO - prev_RMO == 1) {
		dir = LEFT;
	}
	else if (curr_RMO - prev_RMO == -1) {
		dir = RIGHT;
	}
	else if (curr_RMO - prev_RMO > 1) {
		dir = UP;
	}
	else if (curr_RMO - prev_RMO < -1) {
		dir = DOWN;
	}
	else {
		//somethings wrong
	}
	int index = height/2;
	for (int i = 0; i < unitList.size(); i++) {
		switch (dir) {
		case UP:
			//place char at top, mob at bot
			if (unitList[i]->getType() == CHARACTER) {
				while (roomMap[index].type != NOTHING && notWallAdj(index))
					index = rand() % width;
				roomMap[index].type = CHARACTER;
				unitList[i]->setRMO(index);
			}
			else {
				while (roomMap[index + (height - 1)*width].type != NOTHING && notWallAdj(index))
					index = rand() % width;
				roomMap[index + (height - 1)*width].type = ENEMY;
				unitList[i]->setRMO(index + (height - 1)*width);
			}
			break;
		case LEFT:
			//place char at left, mob on right
			if (unitList[i]->getType() == CHARACTER) {
				while (roomMap[index*width].type != NOTHING && notWallAdj(index))
					index = rand() % height;
				roomMap[index*width].type = CHARACTER;
				unitList[i]->setRMO(index*width);
			}
			else {
				while (roomMap[(index + 1)*width - 1].type != NOTHING && notWallAdj(index))
					index = rand() % height;
				roomMap[(index + 1)*width - 1].type = ENEMY;
				unitList[i]->setRMO((index + 1)*width - 1);
			}
			break;
		case DOWN:
			//place char on bot, mob on top
			if (unitList[i]->getType() == CHARACTER) {
				while (roomMap[index + (height - 1)*width].type != NOTHING && notWallAdj(index))
					index = rand() % width;
				roomMap[index + (height - 1)*width].type = CHARACTER;
				unitList[i]->setRMO(index + (height - 1)*width);
			}
			else {
				while (roomMap[index].type != NOTHING && notWallAdj(index))
					index = rand() % width;
				roomMap[index].type = ENEMY;
				unitList[i]->setRMO(index);
			}
			break;
		case RIGHT:
			//place char on right, mob on left
			if (unitList[i]->getType() == CHARACTER) {
				while (roomMap[(index + 1)*width - 1].type != NOTHING && notWallAdj(index))
					index = rand() % height;
				roomMap[(index + 1)*width - 1].type = CHARACTER;
				unitList[i]->setRMO((index + 1)*width - 1);
			}
			else {
				while (roomMap[index*width].type != NOTHING && notWallAdj(index))
					index = rand() % height;
				roomMap[index*width].type = ENEMY;
				unitList[i]->setRMO(index*width);
			}
			break;
		}
	}
	
}

bool Room::notWallAdj(int index) {
	int count = 0;
	if (index%width > 0) {
		if (roomMap[index - 1].type == WALL)
			count++;
	}
	else
		count++;
	if (index/width > 0) {
		if (roomMap[index - width].type == WALL)
			count++;
	}
	else
		count++;
	if (index%width < width - 1) {
		if (roomMap[index + 1].type == WALL)
			count++;
	}
	else
		count++;
	if (index/width < height - 1) {
		if (roomMap[index + width].type == WALL)
			count++;
	}
	else
		count++;
	if (count == 4)
		return false;
	return true;
}

void Room::passControl() {
	init_index++;
	init_index %= unitList.size();
	//update cds for this chars abs
	getCurrUnit()->resetTurn();
	getCurrUnit()->updateAdj();

}

void Room::clearRange() {
	for (int i = 0; i < roomMap.size(); i++) {
		roomMap[i].color = NORMAL;
	}

}

void Room::checkState() {
	if (char_count <= 0) {
		state = GAMEOVER;
		delete gParty;
		gParty = NULL;
		delete room;
		room = NULL;
	}
	else if (enemy_count <= 0) {
		state = DUNGEON;
		delete room;
		room = NULL;
		r = 1275;
		ab = NOPE;
	}
}