#include "Ability.h"
#include "LButton.h"
#include "Room.h"

std::unordered_map<std::string, Ability> abMap;

void(*click_handler)(int index) = NULL;

void loadAbilityMap(){
	//move
	abMap["Move"] = Ability("Move", "Move the character", FREE, 1, 6, 0, 0, moveButton, moveClick);
	abMap["Greataxe"] = Ability("Greataxe", "Attack with a greataxe", ACTION, 1, 1, 8, 0, greatAxeButton, greatAxeClick);
	
}

//generate paths
void bloom(std::vector<int> &prevVec, int start, int end) {
	int width = room->getWidth();
	std::queue<int> prevQ;
	prevQ.push(end);
	while(!prevQ.empty()) {
		int x = prevQ.front() % width;
		int y = prevQ.front() / width;
		if (x == start%width && y == start/width)
			return;
		prevQ.pop();
		if (room->getTile(x, y)->type != RANGE)
			continue;
		//add all adjacent non in prevvec tiles
		if (x > 0) {
			if (prevVec[x - 1 + y * width] == -1) {
				prevVec[x - 1 + y * width] = x + y * width;
				prevQ.push(x - 1 + y * width);
			}
		}
		if (y > 0) {
			if (prevVec[x + (y-1) * width] == -1) {
				prevVec[x + (y-1) * width] = x + y * width;
				prevQ.push(x + (y-1) * width);
			}
		}
		if (x < room->getWidth() - 1) {
			if (prevVec[x + 1 + y * width] == -1) {
				prevVec[x + 1 + y * width] = x + y * width;
				prevQ.push(x + 1 + y * width);
			}
		}
		if (y < room->getHeight() - 1) {
			if (prevVec[x + (y + 1) * width] == -1) {
				prevVec[x + (y + 1) * width] = x + y * width;
				prevQ.push(x + (y + 1) * width);
			}
		}
	}


}

std::vector<int> getPath(int end, int start) {
	std::vector<int> retvec;
	int width = room->getWidth();
	int curr_x = start % width;
	int curr_y = start / width;
	int next_x = end % width;
	int next_y = end / width;
	int RMO = start;
	int distance = std::abs(curr_x - next_x) + std::abs(curr_y - next_y);
	/*std::vector<bool> visited(width*room->getHeight(), false);
	pathHelper(curr_x, curr_y, next_x, next_y, distance, retvec, visited);*/
	std::vector<int> prevvec(width*room->getHeight(), -1);
	bloom(prevvec, start, end);
	while (RMO != end) {
		retvec.push_back(RMO);
		RMO = prevvec[RMO];
	}
	retvec.push_back(RMO);
	return retvec;
}

Ability::Ability() {

}

Ability::Ability(std::string name_, std::string info_, AbilityType type_, int cd, int l, int dice, int dmg, void(*button)(int index), void(*click)(int index)) {
	name = name_;
	info = info_;
	type = type_;
	cooldown = cd;
	length = l;
	num_dice = dice;
	dmg_dice = dmg;
	button_handler = button;
	click_handler = click;
}

int Ability::rollDamage(int dmg_mod) {
	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = dmg_mod;
	for (int i = 0; i < num_dice; i++)
		roll += 1 + rand() % dmg_dice;
	return roll;
}
//click handlers for abilities

void moveClick(int index) {
	int i;
	int width = room->getWidth();
	Unit* curr = room->getCurrUnit();
	int curr_x = curr->getRMO() % width;
	int curr_y = curr->getRMO() / width;
	int next_x = index % width;
	int next_y = index / width;
	int distance = std::abs(curr_x - next_x) + std::abs(curr_y - next_y);
	curr->setMoveLeft(curr->getMoveLeft() - distance);
	std::vector<int> RMO_list = getPath(index, curr->getRMO());
	room->clearRange();
	room->getTile(curr->getRMO() % width, curr->getRMO() / width)->type = NOTHING;
	for (i = 0; i < RMO_list.size(); i++) {
		//SDL_Delay(100);
		curr->setRMO(RMO_list[i]);
		room->getTile(RMO_list[i] % width, RMO_list[i] / width)->type = CHARACTER;
		drawRoom();
		room->getTile(RMO_list[i] % width, RMO_list[i] / width)->type = NOTHING;
		SDL_RenderPresent(gRenderer);
		Sleep(150);
	}
	room->getTile(curr->getRMO() % width, curr->getRMO() / width)->type = CHARACTER;
	room->clearRange();
	if (curr->getMoveLeft() > 0)
		moveButton(0);
	else
		ab = NOPE;
}

void greatAxeClick(int index) {

}