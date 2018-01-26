#include "Mob.h"

std::unordered_map<std::string, Mob> mobMap;

//load all mobs into the map
void loadMobs() {
	mobMap["Skeleton"] = Mob(13, 13, 6, 1, 4, 2, mobClips[SKELETON], "Skeleton", 0, 2, 2, -2, -2, -3);
}


Mob::Mob(){}

Mob::Mob(int AC_, int HP_, int dmg_, int dice_, int atk_mod_, int dmg_mod_, SDL_Rect icon, std::string name_, int s, int d, int c, int i, int w, int ch){
	type = ENEMY;
	AC = AC_;
	dmg = dmg_;
	dice = dice_;
	icon_50 = icon;
	name = name_;
	atk_mod = atk_mod_;
	dmg_mod = dmg_mod_;
	str = s;
	dex = d;
	con = c;
	intel = i;
	wis = w;
	cha = ch;
}

SDL_Rect Mob::getIcon50() {

	return icon_50;
}