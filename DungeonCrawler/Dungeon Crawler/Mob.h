#ifndef MOB_H
#define MOB_H

#include "lib.h"
#include "LButton.h"

class Mob: public Unit {
public:
	Mob();
	Mob(int AC_,int HP_,int dmg_, int dice_, int atk_mod_, int dmg_mod_, SDL_Rect icon, std::string name_, int s, int d, int c, int i, int w, int ch);

	/* using globals, update threat */
	void updateThreat();

	/* using the updates threat, choose target */
	void updateTarget();

	/* move the enemy mob */
	bool move();

	/* attack the target, returns true on kill */
	bool attack();

	SDL_Rect getIcon50();

	int getDex() { return dex; }
	int getMaxHP() { return max_HP; }
	int getHP() { return HP; }
private:
	int target_index;
	std::vector<int> threat;
	std::string name;
	SDL_Rect icon_50;
	int max_HP;
	int HP;
	int AC;
	int atk_mod;
	int dmg_mod;
	int dice;
	int dmg;
	int str, dex, con, intel, wis, cha;
};

extern std::unordered_map<std::string, Mob> mobMap;

void loadMobs();
#endif

