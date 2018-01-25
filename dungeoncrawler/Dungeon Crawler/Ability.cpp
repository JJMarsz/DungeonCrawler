#include "Ability.h"

std::unordered_map<std::string, Ability> abMap;

void loadAbilityMap(){
	//move
	abMap["Move"] = Ability("Move", "Move the character", FREE, 1, 6, 0, 0);

	
}

Ability::Ability() {

}
Ability::Ability(std::string name_, std::string info_, AbilityType type_, int cd, int l, int dice, int dmg) {
	name = name_;
	info = info_;
	type = type_;
	cooldown = cd;
	length = l;
	num_dice = dice;
	dmg_dice = dmg;
}