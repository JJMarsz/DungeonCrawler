#ifndef QUEST_H
#define QUEST_H

#include "lib.h"
#include "dungeon.h"
#include "LTexture.h"

class Enemy {
public:
	/* using globals, update threat */
	void updateThreat();

	/* using the updates threat, choose target */
	void updateTarget();

	/* move the enemy mob */
	bool move();

	/* attack the target, returns true on kill */
	bool attack();


private:
	int target_index;
	std::vector<int> threat;
	LTexture sprite;
	int x;
	int y;
	int HP;
	int AC;
	int atk_mod;
	int atk_max;
	int atk_min;
};

class Encounter {
public:
	EncounterType getType();
	void setType(EncounterType type_);
private:
	std::vector<Enemy> moblist;
	EncounterType type;
};

class Quest{
public:
	Quest();
	Quest(std::string name_, std::string info_, int gold, int xp, Difficulty diff_);
private:
	std::vector<Encounter> normal_enemy;
	std::vector<Encounter> special;
	std::string name;
	std::string info;
	int gold_reward;
	int xp_reward;
	Difficulty diff;
};

/* generates quests and fills quest array */
void loadQuests();

extern std::vector<Quest> current_quests;

#endif

