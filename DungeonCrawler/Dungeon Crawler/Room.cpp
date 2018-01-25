#include "Room.h"
#include "Quest.h"

Room* room = NULL;

//used to sort in descending order
bool wayToSort(int i, int j) { return i > j; }

Room::Room(){
	init_index = 0;
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

}

int Room::getWidth() { return width; }
int Room::getHeight() { return height; }
RoomTile* Room::getTile(int x, int y) { return &roomMap[x + y * width]; }
Unit* Room::getCurrUnit() { return unitList[init_index]; }
int Room::getInitIndex() { return init_index; }
std::vector<Unit*>* Room::getInititiveOrder() { return &unitList; }

void Room::rollInit() {
	//only dealing with chars for now
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	unitList.resize(MAX_CHAR);
	std::unordered_map<int, Unit*> map;
	std::vector<int> init;
	int i, mod = 0;
	if (current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth()))
		mod = 5;
	for (i = 0; i < 3; i++) {
		int roll = rand() % 20 + mod + gParty.getChar(i)->getDex();
		while (map.find(roll) != map.end())
			roll--;
		Unit* newboi = gParty.getChar(i);
		map[roll] = newboi;
		init.push_back(roll);
	}
	//then add mobs

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
	Tile* curr = current_dungeon.getTile(gParty.getX() + gParty.getY()*dung_width);
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
			if (unitList[i]->getType() == CHAR) {
				while (roomMap[index].type != NOTHING)
					index = rand() % width;
				roomMap[index].type = CHAR;
				unitList[i]->setRMO(index);
			}
			else {
				while (roomMap[index + (height - 1)*width].type != NOTHING)
					index = rand() % width;
				roomMap[index + (height - 1)*width].type = ENEMY;
				unitList[i]->setRMO(index + (height - 1)*width);
			}
			break;
		case LEFT:
			//place char at left, mob on right
			if (unitList[i]->getType() == CHAR) {
				while (roomMap[index*width].type != NOTHING)
					index = rand() % height;
				roomMap[index*width].type = CHAR;
				unitList[i]->setRMO(index*width);
			}
			else {
				while (roomMap[(index + 1)*width - 1].type != NOTHING)
					index = rand() % height;
				roomMap[(index + 1)*width - 1].type = ENEMY;
				unitList[i]->setRMO((index + 1)*width - 1);
			}
			break;
		case DOWN:
			//place char on bot, mob on top
			if (unitList[i]->getType() == CHAR) {
				while (roomMap[index + (height - 1)*width].type != NOTHING)
					index = rand() % width;
				roomMap[index + (height - 1)*width].type = CHAR;
				unitList[i]->setRMO(index + (height - 1)*width);
			}
			else {
				while (roomMap[index].type != NOTHING)
					index = rand() % width;
				roomMap[index].type = ENEMY;
				unitList[i]->setRMO(index);
			}
			break;
		case RIGHT:
			//place char on right, mob on left
			if (unitList[i]->getType() == CHAR) {
				while (roomMap[(index + 1)*width - 1].type != NOTHING)
					index = rand() % height;
				roomMap[(index + 1)*width - 1].type = CHAR;
				unitList[i]->setRMO((index + 1)*width - 1);
			}
			else {
				while (roomMap[index*width].type != NOTHING)
					index = rand() % height;
				roomMap[index*width].type = ENEMY;
				unitList[i]->setRMO(index*width);
			}
			break;
		}
	}
	
}

void Room::passControl() {
	init_index++;
	init_index %= unitList.size();
	//update cds for this chars abs
	getCurrUnit()->resetMove();
}

void Room::clearRange() {
	for (int i = 0; i < roomMap.size(); i++) {
		if (roomMap[i].type == RANGE)
			roomMap[i].type = NOTHING;
	}

}

void Room::move(int index) {
	roomMap[getCurrUnit()->getRMO()].type = NOTHING;
	roomMap[index].type = CHAR;
}