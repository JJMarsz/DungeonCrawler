#include "Room.h"
#include "Quest.h"

Room* room = NULL;

bool wayToSort(int i, int j) { return i > j; }
Room::Room(){
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
			roomMap[i].special = true;
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
Unit* Room::getCurrUnit() { return *currUnit; }
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
	//set up current top unit
	currUnit = unitList.begin();
}

void Room::passControl() {

}
