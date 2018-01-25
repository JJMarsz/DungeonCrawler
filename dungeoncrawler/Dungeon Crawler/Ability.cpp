#include "Ability.h"
#include "LButton.h"
#include "Room.h"

std::unordered_map<std::string, Ability> abMap;

void(*click_handler)(int index) = NULL;

void loadAbilityMap(){
	//move
	abMap["Move"] = Ability("Move", "Move the character", FREE, 1, 6, 0, 0, moveButton, moveClick);

	
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
	Unit* curr = room->getCurrUnit();
	int curr_x = curr->getRMO() % room->getWidth();
	int curr_y = curr->getRMO() / room->getHeight();
	int next_x = index % room->getWidth();
	int next_y = index / room->getHeight();
	int distance = std::abs(curr_x - next_x) + std::abs(curr_y - next_y);
	curr->setMoveLeft(curr->getMoveLeft() - distance);
	room->move(index);
	curr->setRMO(index);
	room->clearRange();
	if(curr->getMoveLeft() > 0)
		moveButton(0);
}