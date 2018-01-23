#ifndef ENCOUNTER_H
#define ENCOUNTER_H

#include "lib.h"

#define CHOICE_MENU_WIDTH	454
#define CHOICE_MENU_HEIGHT	124

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

#define NUM_LOOT	1
enum LootIndex {
	GOLD
};

#define NUM_CHOICE	10
enum ChoiceIndex {
	WELL,
	OBELISK,
	CHEST,
	TOME,
	STR,
	DEX,
	CON,
	INT,
	WIS,
	CHA
};

class EncounterList {
public:
	EncounterList();
	func getTrap(TrapIndex index);
	func getInfo(InfoIndex index);
	func getLoot(LootIndex index);
	func getChoice(ChoiceIndex index);
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

void goldLoot();

void well();
void mysticObelisk();
void magicChest();
void ancientTome();
void testStr();
void testDex();
void testCon();
void testInt();
void testWis();
void testCha();
#endif