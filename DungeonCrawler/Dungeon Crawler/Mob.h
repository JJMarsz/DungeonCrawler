#ifndef MOB_H
#define MOB_H

#include "lib.h"
#include "LButton.h"

class Mob: public Unit {
public:
	Mob();

	/* using globals, update threat */
	void updateThreat();

	/* using the updates threat, choose target */
	void updateTarget();

	/* move the enemy mob */
	bool move();

	/* attack the target, returns true on kill */
	bool attack();

	SDL_Rect getIcon50();
private:
	int target_index;
	std::vector<int> threat;
	SDL_Rect icon_50;
	LTexture sprite;
	int x;
	int y;
	int HP;
	int AC;
	int atk_mod;
	int atk_max;
	int atk_min;
};

#endif

