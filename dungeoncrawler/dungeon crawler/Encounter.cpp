#include "Encounter.h"
#include "dungeon.h"
#include "Quest.h"

std::queue<std::string> msg_queue;
//initialize every encounter
EncounterList::EncounterList() {
	//trap setup
	trapList.resize(NUM_TRAP);
	trapList[BASIC] = basicTrap;
}

func EncounterList::getTrap(TrapIndex index) { return trapList[index]; }
void handleEncounter() {
	//dispatcher to appropriate encounter handler
	Tile* tile = current_dungeon.getTile(gParty.getX() + gParty.getY()*current_dungeon.getWidth());
	//for now, only custom encounters will be boss and mobs, everything else will be generic
	switch (tile->getType()) {
	case TRAP:
		current_quests[quest_index].fetchTrap((TrapIndex)tile->getIndex())();
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
	case EASY:
		DC = 12;
		dice = 1;
		break;
	case MED:
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
	srand(time(NULL));
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
	current_dungeon.scoutTile(bosstile->getX() + bosstile->getY()*current_dungeon.getWidth());
	msg_queue.push("A scroll informs you where the boss is within the dungeon.");
}

void mobReveal() {
	bool seen = false;
	std::vector<Tile*>* list = current_dungeon.getMob();
	for (int i = 0; i < list->size(); i++) {

	}
}

void areaReveal() {
	int x, y;
	int width = current_dungeon.getWidth();
	int height = current_dungeon.getHeight();
	std::vector<Tile*> options;
	//find valid 2x2 areas
	for (x = 0; x < height; x++) {
		for (y = 0; y < width; y++) {
			bool encounter = false;
			//check adjacent
			if (current_dungeon.getTile(x + y * width)->getType() != PATH && current_dungeon.getTile(x + y * width)->getType() != NONE && 
				current_dungeon.getTile(x + y * width)->getType() != DEADEND && !current_dungeon.getSightStatus(x + y*width))
				encounter = true;
			if (x < width - 1) {
				if (current_dungeon.getTile(x + 1 + y * width)->getType() != PATH && current_dungeon.getTile(x + 1 + y * width)->getType() != NONE 
					&& !current_dungeon.getSightStatus(x + 1 + y * width) && current_dungeon.getTile(x + 1 + y * width)->getType() != DEADEND) {
					encounter = true;
				}
				if (y < height - 1) {
					if (current_dungeon.getTile(x + (y + 1) * width)->getType() != PATH && current_dungeon.getTile(x + (y + 1) * width)->getType() != NONE 
						&& !current_dungeon.getSightStatus(x + (y + 1) * width) && current_dungeon.getTile(x + (y + 1) * width)->getType() != DEADEND) {
						encounter = true;
					}
					if (current_dungeon.getTile(x + 1 + (y + 1) * width)->getType() != PATH && current_dungeon.getTile(x + 1 + (y + 1) * width)->getType() != NONE 
						&& !current_dungeon.getSightStatus(x + 1 + (y + 1) * width) && current_dungeon.getTile(x + 1 + (y + 1) * width)->getType() != DEADEND) {
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
	current_dungeon.scoutTile(x + y * width);
	current_dungeon.scoutTile(x + 1 + y * width);
	current_dungeon.scoutTile(x + (y+1) * width);
	current_dungeon.scoutTile(x + 1 + (y+1) * width);
	msg_queue.push("You found a map that details an area within the dungeon!");
	return;
}

//choice handlers

void well() {
	//heals or damages
}

void mysticObelisk() {
	//gain an xp or loose an xp

}

void magicChest() {
	//gain gold or take dmg

}

void ancientTome() {
	//gain +1 in highest stat

}


//loot handlers
