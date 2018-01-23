#ifndef QUEST_H
#define QUEST_H

#include "lib.h"
#include "LTexture.h"
#include "dungeon.h"

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


class Quest{
public:
	Quest();
	Quest(std::string name_, std::string info_, int gold, int xp, Difficulty diff_);
	int getGold();
	int getXP();
	std::string getTitle();
	std::string getInfo();
	Difficulty getDiff();
	void loadTrap(TrapIndex i);
	void loadInfo(InfoIndex i);
	void loadLoot(LootIndex i);
	void loadChoice(ChoiceIndex i);
	func fetchTrap(TrapIndex index);
	func fetchInfo(InfoIndex index);
	func fetchLoot(LootIndex index);
	func fetchChoice(ChoiceIndex index);
private:
	/* Boss fight encounter */
	void(*Boss)();
	/* Regular mob fight encounter */
	std::vector<void(*)()> enemyEnc;
	/* Info/Loot on dungeon or special dungeon move */
	std::vector<void(*)()> infoEnc;
	/* Choice on one of party members */
	std::vector<void(*)()> choiceEnc;
	/* Things to generate in dead ends */
	std::vector<void(*)()> deadEnc;
	/* Traps */
	std::vector<void(*)()> trapEnc;
	/* Loot */
	std::vector<void(*)()> lootEnc;

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


extern EncounterList encounters;
extern std::vector<Quest> current_quests;
extern QuestList quests;

#endif

