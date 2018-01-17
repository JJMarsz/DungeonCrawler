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
	//Encounter(EncounterType type_);
	EncounterType getType();
	void setType(EncounterType type_);
private:
	/* For combat encounters */
	//std::vector<Enemy> moblist;
	/* for Info/Loot encounters */
	//
	/*  */

	EncounterType type;
};

class Quest{
public:
	Quest();
	Quest(std::string name_, std::string info_, int gold, int xp, Difficulty diff_);
	int getGold();
	int getXP();
	std::string getTitle();
	std::string getInfo();
	Difficulty getDiff();
private:
	/* Boss fight encounter */
	Encounter Boss;
	/* Regular mob fight encounter */
	std::vector<Encounter> enemyEnc;
	/* Info/Loot on dungeon or special dungeon move */
	std::vector<Encounter> infoEnc;
	/* Choice on one of party members */
	std::vector<Encounter> choiceEnc;
	/* Things to generate in dead ends */
	std::vector<Encounter> deadEnc;
	std::string name;
	std::string info;
	int gold_reward;
	int xp_reward;
	Difficulty diff;
};

/* generates quests and fills quest array */
void initQuests();

class QuestList {
public:
	struct QuestDesc {
		Quest q;
		bool show;
		bool complete;
	};
	QuestList();
	bool isAvailable(int index);
	Quest showQuest(int index);
	void completeQuest(std::string name);
	void unShowQuest();
	void getQuests();
	void getEasyQuest();
	void getMedQuest();
	void getHardQuest();

private:
	std::vector<QuestDesc> list;
	int easyQuestIndex;
	int medQuestIndex;
	int hardQuestIndex;

};
extern std::vector<Quest> current_quests;
extern QuestList quests;

#endif

