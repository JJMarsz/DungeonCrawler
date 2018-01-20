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
	Tile* tile = current_dungeon.getTile(gParty.getX() + gParty.getY()*current_dungeon.getWidth());
	if (!current_dungeon.getScouted(gParty.getX() + gParty.getY()*current_dungeon.getWidth())) {
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
		//have scouted the trap, one char attempts a disarm
		msg_queue.push("The party expertly dodged a trap.");
	}

}
