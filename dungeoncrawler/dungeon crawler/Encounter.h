#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include "lib.h"


enum EncounterType {
	NONE,
	PATH,
	LOOT,
	INFO,
	BOSS,
	MOB,
	DEADEND,
	BARRIER,
	CHOICE,
	TRAP
};

#define NUM_TRAP	1
enum TrapIndex {
	BASIC
};

#define NUM_INFO	6
enum InfoIndex {
	AREA_REVEAL,
	BOSS_REVEAL,
	MOB_REVEAL,
	LOOT_REVEAL,
	CHOICE_REVEAL,
	TRAP_REVEAL
};

class EncounterList {
public:
	EncounterList();
	func getTrap(TrapIndex index);
	func getInfo(InfoIndex index);
private:
	//these vectors contain pointers to every encounter function within the game
	//quests will index into these and copy the ptrs to use during population
	std::vector<void(*)()> bossList;
	std::vector<void(*)()> trapList;
	std::vector<void(*)()> mobList;
	std::vector<void(*)()> infoList;
	std::vector<void(*)()> lootList;
	std::vector<void(*)()> choiceList;
};


extern std::queue<std::string> msg_queue;

void handleEncounter();
//handler defenitions

void basicTrap();

void areaReveal();
void bossReveal();
void mobReveal();
void lootReveal();
void choiceReveal();
void trapReveal();
#endif