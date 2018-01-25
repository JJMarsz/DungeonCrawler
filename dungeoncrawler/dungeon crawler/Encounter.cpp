#include "Encounter.h"
#include "dungeon.h"
#include "Quest.h"
#include "Room.h"

std::queue<std::string> msg_queue;
//initialize every encounter
EncounterList::EncounterList() {
	//trap setup
	trapList.resize(NUM_TRAP);
	trapList[BASIC] = basicTrap;

	//info setup
	infoList.resize(NUM_INFO);
	infoList[AREA_REVEAL] = areaReveal;
	infoList[BOSS_REVEAL] = bossReveal;
	infoList[MOB_REVEAL] = mobReveal;
	infoList[LOOT_REVEAL] = lootReveal;
	infoList[CHOICE_REVEAL] = choiceReveal;
	infoList[TRAP_REVEAL] = trapReveal;

	//loot setup
	lootList.resize(NUM_LOOT);
	lootList[GOLD] = goldLoot;

	//choice setup
	choiceList.resize(NUM_CHOICE);
	choiceList[OBELISK] = mysticObelisk;
	choiceList[WELL] = well;
	choiceList[CHEST] = magicChest;
	choiceList[TOME] = ancientTome;
	choiceList[STR] = testStr;
	choiceList[DEX] = testDex;
	choiceList[CON] = testCon;
	choiceList[INT] = testInt;
	choiceList[WIS] = testWis;
	choiceList[CHA] = testCha;

	//mob setup
	mobList.resize(NUM_MOB);
	mobList[TEST] = test;
}

func EncounterList::getTrap(TrapIndex index) { return trapList[index]; }
func EncounterList::getInfo(InfoIndex index) { return infoList[index]; }
func EncounterList::getLoot(LootIndex index) { return lootList[index]; }
func EncounterList::getChoice(ChoiceIndex index) { return choiceList[index]; }
func EncounterList::getMob(MobIndex index) { return mobList[index]; }

void handleEncounter() {
	//dispatcher to appropriate encounter handler
	Tile* tile = current_dungeon.getTile(gParty.getX() + gParty.getY()*current_dungeon.getWidth());
	//for now, only custom encounters will be boss and mobs, everything else will be generic
	switch (tile->getType()) {
	case TRAP:
		current_quests[quest_index].fetchTrap((TrapIndex)tile->getIndex())();
		break;
	case INFO:
		current_quests[quest_index].fetchInfo((InfoIndex)tile->getIndex())();
		break;
	case LOOT:
		current_quests[quest_index].fetchLoot((LootIndex)tile->getIndex())();
		break;
	case CHOICE:
		current_quests[quest_index].fetchChoice((ChoiceIndex)tile->getIndex())();
		break;
	case MOB:
		current_quests[quest_index].fetchMob((MobIndex)tile->getIndex())();
		break;
	default:
		break;
	}
}




/* Handlers for all the encounters, seperated by type */

//trap handlers
void basicTrap() {
	int dmg_roll=0, dice, DC, i, dex_roll;
	switch (current_quests[quest_index].getDiff()) {
		std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
			std::chrono::system_clock::now().time_since_epoch()
			);
		srand(ms.count());//random seed
	case EASY:
		DC = 12;
		dice = 1;
		break;
	case MEDIUM:
		DC = 13;
		dice = 3;
		break;
	case HARD:
		DC = 15;
		dice = 5;
		break;
	}
	for (i = 0; i < dice; i++)
		dmg_roll += (rand() % 4) + 1;
	//3 scenarios
	if (!current_dungeon.perceptionCheck()) {
		//not scouted,all char rolls dex check
		for (i = 0; i < 3; i++) {
			dex_roll = rand() % 20 + 1 + gParty.getChar(i)->getDex();
			if (dex_roll <= DC) {
				gParty.getChar(i)->damage(dmg_roll);
			}
			else {
				gParty.getChar(i)->damage(dmg_roll/2);
			}

		}
		msg_queue.push("The party didn't notice a trap and was blindsided!");
	}
	else {
		msg_queue.push("The party expertly dodged a trap.");
	}

}

//info handlers

void bossReveal() {
	//just reveal boss
	Tile* bosstile = current_dungeon.getBoss();
	current_dungeon.scoutTile(bosstile->getX() + bosstile->getY()*current_dungeon.getWidth(), false);
	msg_queue.push("A scroll informs you where the boss is within the dungeon.");
}

void mobReveal() {
	std::vector<Tile*>* list = current_dungeon.getMob();
	for (int i = 0; i < list->size(); i++) {
		if (!current_dungeon.getScouted(list->at(i)->getX() + list->at(i)->getY()*current_dungeon.getWidth())) {
			current_dungeon.scoutTile(list->at(i)->getX() + list->at(i)->getY()*current_dungeon.getWidth(), false);
			msg_queue.push("The party finds a note written by someone detailing where some enemies might be.");
			return;
		}
	}
	msg_queue.push("The party finds redundant information about some enemies.");
}

void lootReveal() {
	std::vector<Tile*>* list = current_dungeon.getLoot();
	for (int i = 0; i < list->size(); i++) {
		if (!current_dungeon.getScouted(list->at(i)->getX() + list->at(i)->getY()*current_dungeon.getWidth())) {
			current_dungeon.scoutTile(list->at(i)->getX() + list->at(i)->getY()*current_dungeon.getWidth(), false);
			msg_queue.push("The party discovers a treasure map that leads somewhere within this dungeon!");
			return;
		}
	}
	msg_queue.push("The party found info on loot that was already uncovered.");
}

void choiceReveal() {
	std::vector<Tile*>* list = current_dungeon.getChoice();
	for (int i = 0; i < list->size(); i++) {
		if (!current_dungeon.getScouted(list->at(i)->getX() + list->at(i)->getY()*current_dungeon.getWidth())) {
			current_dungeon.scoutTile(list->at(i)->getX() + list->at(i)->getY()*current_dungeon.getWidth(), false);
			msg_queue.push("The party finds a clue pertaining towards a mystical object within the dungeon.");
			return;
		}
	}
	msg_queue.push("The party finds info on a mystical object that they already dealt with.");
}

void trapReveal() {
	std::vector<Tile*>* list = current_dungeon.getTrap();
	for (int i = 0; i < list->size(); i++) {
		if (!current_dungeon.getScouted(list->at(i)->getX() + list->at(i)->getY()*current_dungeon.getWidth())) {
			current_dungeon.scoutTile(list->at(i)->getX() + list->at(i)->getY()*current_dungeon.getWidth(), false);
			msg_queue.push("The party finds blueprints for a trap built within the dungeon.");
			return;
		}
	}
	msg_queue.push("The party finds blueprints on a trap that was already passed.");
}

void areaReveal() {
	int x, y;
	int width = current_dungeon.getWidth();
	int height = current_dungeon.getHeight();
	std::vector<Tile*> options;
	//find valid 2x2 areas
	for (x = 0; x < width; x++) {
		for (y = 0; y < height; y++) {
			bool encounter = false;
			//check adjacent
			if (current_dungeon.getTile(x + y * width)->getType() != PATH && current_dungeon.getTile(x + y * width)->getType() != NONE && 
				current_dungeon.getTile(x + y * width)->getType() != DEADEND && !current_dungeon.getSightStatus(x + y*width) && current_dungeon.getTile(x + y * width)->getType() != BOSS)
				encounter = true;
			if (x < width - 1) {
				if (current_dungeon.getTile(x + 1 + y * width)->getType() != PATH && current_dungeon.getTile(x + 1 + y * width)->getType() != NONE 
					&& !current_dungeon.getSightStatus(x + 1 + y * width) && current_dungeon.getTile(x + 1 + y * width)->getType() != DEADEND 
					&& current_dungeon.getTile(x + 1 + y * width)->getType() != BOSS) {
					encounter = true;
				}
				if (y < height - 1) {
					if (current_dungeon.getTile(x + (y + 1) * width)->getType() != PATH && current_dungeon.getTile(x + (y + 1) * width)->getType() != NONE 
						&& !current_dungeon.getSightStatus(x + (y + 1) * width) && current_dungeon.getTile(x + (y + 1) * width)->getType() != DEADEND
						&& current_dungeon.getTile(x + (y + 1) * width)->getType() != BOSS) {
						encounter = true;
					}
					if (current_dungeon.getTile(x + 1 + (y + 1) * width)->getType() != PATH && current_dungeon.getTile(x + 1 + (y + 1) * width)->getType() != NONE 
						&& !current_dungeon.getSightStatus(x + 1 + (y + 1) * width) && current_dungeon.getTile(x + 1 + (y + 1) * width)->getType() != DEADEND
						&& current_dungeon.getTile(x + 1 + (y + 1) * width)->getType() != BOSS) {
						encounter = true;
					}
				}
				else
					continue;
			}
			else
				continue;
			if (encounter == true)
				options.push_back(current_dungeon.getTile(x + y * width));
		}
	}
	srand(time(NULL));
	if (options.size() == 0) {
		msg_queue.push("You found a map that detailed an area you already visited.");
		return;
	}

	int index = rand() % options.size();
	x = options[index]->getX();
	y = options[index]->getY();
	current_dungeon.scoutTile(x + y * width, true);
	current_dungeon.scoutTile(x + 1 + y * width, true);
	current_dungeon.scoutTile(x + (y+1) * width, true);
	current_dungeon.scoutTile(x + 1 + (y+1) * width, true);
	msg_queue.push("You found a map that details an area within the dungeon!");
	return;
}

//choice handlers

void well() {
	//heals or damages
	state = CHOOSE;
	msg_queue.push("The party sees a well in this erie room. It beckons for someone to drink from it.");
	for (int i = 0; i < 4; i++) {
		choiceButtons[i].setHandler(wellHandler);
	}

}

void mysticObelisk() {
	//gain an xp or loose an xp
	state = CHOOSE;
	msg_queue.push("The party comes across a large obelisk. It calls for one of the party members.");
	for (int i = 0; i < 4; i++) {
		choiceButtons[i].setHandler(obHandler);
	}
}

void magicChest() {
	//gain gold or take dmg
	state = CHOOSE;
	msg_queue.push("The party comes across a regular chest. It sits there, patiently.");
	for (int i = 0; i < 4; i++) {
		choiceButtons[i].setHandler(chestHandler);
	}
}

void ancientTome() {
	//gain +1 in highest stat
	state = CHOOSE;
	msg_queue.push("The party comes across an ancient tome. It's contents are limitless.");
	for (int i = 0; i < 4; i++) {
		choiceButtons[i].setHandler(tomeHandler);
	}
}

void testStr() {
	state = CHOOSE;
	msg_queue.push("The party comes across a feat of strength. Completeing it could be rewarding.");
	for (int i = 0; i < 4; i++) {
		choiceButtons[i].setHandler(strHandler);
	}
}

void testDex() {
	state = CHOOSE;
	msg_queue.push("The party comes across a timed trial. Those dexterous enough may be able to complete it.");
	for (int i = 0; i < 4; i++) {
		choiceButtons[i].setHandler(dexHandler);
	}
}

void testCon() {
	state = CHOOSE;
	msg_queue.push("The party comes across a test of fortitude. Resist it's poisen with great constitution to reap the rewards.");
	for (int i = 0; i < 4; i++) {
		choiceButtons[i].setHandler(conHandler);
	}
}

void testInt() {
	state = CHOOSE;
	msg_queue.push("The party comes across a puzzle requiring great intelligence. Solving it could be beneficial.");
	for (int i = 0; i < 4; i++) {
		choiceButtons[i].setHandler(intHandler);
	}
}

void testWis() {
	state = CHOOSE;
	msg_queue.push("The party comes across a test only suitable for the wise. Can solving it render a positive outcome?");
	for (int i = 0; i < 4; i++) {
		choiceButtons[i].setHandler(wisHandler);
	}
}

void testCha() {
	state = CHOOSE;
	msg_queue.push("The party comes across a humonoid statue. Convince it that it is alive with great charisma to receive it's reward.");
	for (int i = 0; i < 4; i++) {
		choiceButtons[i].setHandler(chaHandler);
	}
}

//loot handlers
void goldLoot() {
	//if scouted, add mod 
	//base off of loot table
	int highest_wis = (gParty.getChar(0)->getWis() < gParty.getChar(1)->getWis()) ? gParty.getChar(1)->getWis() : gParty.getChar(0)->getWis();
	highest_wis = (highest_wis < gParty.getChar(2)->getWis()) ? gParty.getChar(2)->getWis() : highest_wis;

	std::chrono::milliseconds ms = std::chrono::duration_cast< std::chrono::milliseconds >(
		std::chrono::system_clock::now().time_since_epoch()
		);
	srand(ms.count());//random seed
	int roll = rand() % 20 + 1;
	//automatic fail
	if (roll == 1) {
		msg_queue.push("There doesn't seem to be anything here.");
		return;
	}
	int x = gParty.getX();
	int y = gParty.getY();
	//what need to be beat
	int lowDC = 12;
	int medDC = 16;
	int highDC = 20;
	int gold = 0;
	if (current_dungeon.getScouted(x + y * current_dungeon.getWidth()))
		roll += 5;
	if (roll + highest_wis > highDC) {
		gold = (rand() % 6) * 5 + 20;
		msg_queue.push("The party finds some gold lying around.");
	}
	else if (roll + highest_wis > medDC) {
		gold = (rand() % 8) * 5 + 40;
		msg_queue.push("The party uncovers a good amount of gold.");
	}
	else if (roll + highest_wis > lowDC) {
		gold = (rand() % 10) * 5 + 60;
		msg_queue.push("The party discovers a trove of gold!");
	}
	else {
		msg_queue.push("There doesn't seem to be anything here.");
	}

	switch (current_quests[quest_index].getDiff()) {
	case MEDIUM:
		gold += 20;
		break;
	case HARD:
		gold += 40;
		break;
	default:
		break;
	}
	gParty.addGold(gold);


}


//mob handlers

void test() {
	state = DUNGEON_ROOM;
	room = new Room;
	room->rollInit();
	room->placeUnits();
}