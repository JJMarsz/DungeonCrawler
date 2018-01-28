#ifndef MOB_H
#define MOB_H

#include "lib.h"
#include "LButton.h"
#include "party.h"

class Mob: public Unit {
public:
	Mob();
	Mob(int AC_,int HP_,int dmg_, int dice_, int atk_mod_, int dmg_mod_, SDL_Rect icon, std::string name_, int s, int d, int c, int i, int w, int ch, int move_);

	/* using globals, update threat and select target player to attack */
	void updateThreat();

	/* attack the target, returns true on kill */
	void attack(int index);
	void moveMob();
	SDL_Rect getIcon50();
	void callHandler() { handler(); }
	void addAttackThreat(int t);
	void clearAttackThreat(int i){threat[i] = 0;}
private:
	int target_index;
	std::vector<int> threat;
	SDL_Rect icon_50;
	int atk_mod;
	int dmg_mod;
	int dice;
	int dmg;
	void(*handler)();
};

extern std::unordered_map<std::string, Mob> mobMap;

void loadMobs();

void meleeMobHandler();

#endif

