#include "Ability.h"
#include "LButton.h"
#include "Room.h"

std::unordered_map<std::string, Ability> abMap;

void(*click_handler)(int index) = NULL;

void loadAbilityMap(){
	//move
	abMap["Move"] = Ability("Move", "Move the character", FREE, 1, 6, 0, 0, moveButton, moveClick);

	
}

std::vector<int> getPath(int end, int start) {
	std::vector<int> retvec;
	int width = room->getWidth();
	int target_x = end % width;
	int target_y = end / width;
	int x = start % width;
	int y = start / width;
	bool switcher = false;
	while (x + y*width != end) {
		if (room->getTile(x, y)->type == RANGE && x + y * width != start && retvec.size() != 0 && x + y * width != end) {
			retvec.pop_back();
			if (retvec.size() > 0) {
				x = retvec.back() % width;
				y = retvec.back() / width;
			}
			else {
				x = start % width;
				y = start / width;
			}
		}
		switch (switcher) {
		case true:
			if (x != target_x) {
				if (x < target_x)
					x++;
				else if(x > target_x)
					x--;
			}
			switcher = false;
			break;
		case false:
			if (y != target_y) {
				if (y < target_y)
					y++;
				else if(y > target_y)
					y--;
			}
			switcher = true;
			break;
		}
		retvec.push_back(x + y * width);
	}
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

//click handlers for abilities

void moveClick(int index) {
	int i;
	int width = room->getWidth();
	Unit* curr = room->getCurrUnit();
	int curr_x = curr->getRMO() % room->getWidth();
	int curr_y = curr->getRMO() / room->getHeight();
	int next_x = index % room->getWidth();
	int next_y = index / room->getHeight();
	int distance = std::abs(curr_x - next_x) + std::abs(curr_y - next_y);
	curr->setMoveLeft(curr->getMoveLeft() - distance);
	std::vector<int> RMO_list = getPath(index, curr->getRMO());
	room->clearRange();
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